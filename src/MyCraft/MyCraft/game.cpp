#include "game.h"
#include "resource_manager.h"

Game::Game(GLuint width, GLuint height)
{
	this->Width = width;
	this->Height = height;

	camera = nullptr;

	world = nullptr;

	mousePicker = nullptr;

	for (int i = 0; i < 1024; i++)
	{
		Keys[i] = false;
	}
	State = GAME_ACTIVE;
}

Game::~Game()
{
	if (camera != nullptr)
		delete camera;
	if (world != nullptr)
		delete world;
}

void Game::Init()
{
	// ����һ�������
	this->camera = new Camera(glm::vec3(3.0f, 8.0f, 3.0f));


	// ��ʱ�ȷ�����������ͶӰ������ΪҪ���� mousePicker ������


	// float projectionMatrix[16];
	// glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
	// �������ʰȡ��
	// this->mousePicker = new MousePicker(this->camera, glm::make_mat4(projectionMatrix));
	this->mousePicker = new MousePicker(this->camera, glm::perspective(65.0f, (float)Width / Height, 0.125f, 100.0f));

	// ������Ϸ�������ɫ����������Դ
	ResourceManager::LoadTexture("textures/grass_block_side.png", false, "tex_side");
	ResourceManager::LoadTexture("textures/Test.png", false, "tex_up");
	ResourceManager::LoadShader("shaders/sky_vertex.glsl", "shaders/sky_fragment.glsl", nullptr, "shader_skybox");

	// �½�һ�� world ����
	this->world = new World();
	// ����������Դ
	this->world->Load();
	// ��ʼ������
	this->world->init();

}

void Game::ProcessInput(GLfloat dt)
{
	dt = 0.01;

	camera->doMovement(this->Keys, world, dt);

	/*
		// �����û���������������λ��
	if (Keys['w'])
		camera->ProcessKeyboard(FORWARD, this->world ,dt);
	if (Keys['s'])
		camera->ProcessKeyboard(BACKWARD, this->world, dt);
	if (Keys['a'])
		camera->ProcessKeyboard(LEFT, this->world, dt);
	if (Keys['d'])
		camera->ProcessKeyboard(RIGHT, this->world, dt);
	if (Keys[' '])
		camera->ProcessKeyboard(JUMP, this->world, dt);
	camera->ProcessKeyboard(IDLE, this->world, dt);
	
	*/

}


void Game::Render()
{

	// ��ʱʹ�ù̶�������Ⱦ�ķ�ʽ������ skyBox ������ɫ����Ⱦ��

	// clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0f, (float)Width / Height, 0.125f, 100.0f);

	// ����ģ�;���
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// �����ӵ�
	camera->setLookAt();

	// ��Ⱦ����
	world->render();

	// glutWireCube(10);

	// �����������
	this->mousePicker->render_ray();

	// ��Ⱦ��Ļ���ĵ�ʮ��
	this->render_cross();

}

void Game::Update(GLfloat dt)
{
	// ���ж��Ƿ���ײ����y�����λ�ã����ж��Ƿ���Ծ
	// camera->updateCamera(this->world, dt);
}

// �ȷ����, ������һ��ѡ�еķ���λ�ã�Ϊ���ܹ���ѡ�е�ʱ�������߿�
static int block_last_x = 0;	
static int block_last_y = 0;
static int block_last_z = 0;

void Game::MouseMoveCallback(int xpos, int ypos)
{
	this->camera->camera_mouse_callback(xpos, ypos);

	// update mouse picker
	this->mousePicker->update(xpos, ypos, Width, Height);


	// ---------------------------------------------------------
	// TODO: ��������ʰȡ
	int distance = 8;			// �ܹ����Ŷ�Զ

	glm::vec3 ray = this->mousePicker->getCurrentRay();
	int x, y, z;
	for (int u = 0; u < distance; u++) {
		x = roundf(ray.x * u + camera->Position.x);
		y = roundf(ray.y * u + camera->Position.y);
		z = roundf(ray.z * u + camera->Position.z);
		if (world->get_map(x, y, z) ) {
			if (block_last_x == x && block_last_y == y && block_last_z == z) {
				// cout << "�Ѿ�ѡ����"<< endl;
			}
			else {
				world->unpick_block(block_last_x, block_last_y, block_last_z);
				world->pick_block(x, y, z);
				block_last_x = x;
				block_last_y = y;
				block_last_z = z;
			}
			break;
		}
	}
	// --------------------------------------------
}

void Game::MouseClickCallback(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			// �����Ƿ��÷��鹦��
			// ���˵�ǰѡ�еķ��飬�ж��������뷽�����һ�����ཻ
			// cout << block_last_x << " " << block_last_y << " " << block_last_z << endl;
			glm::vec3 ray = this->mousePicker->getCurrentRay();
			glm::vec3 pos = glm::vec3(block_last_x, block_last_y, block_last_z);
			// ��ȡ�ཻ����
			glm::vec3 p = this->mousePicker->Intersect(ray, camera->Position, block_last_x, block_last_y, block_last_z);
			if (p == glm::vec3(0, 0, 0)) {
				return;
			}
			pos = pos + p;

			if (!world->get_map(pos.x, pos.y, pos.z)) {	// ��ǰλ��û�з���
				if (glm::abs(camera->Position.x - pos.x) < 1
					&& glm::abs(camera->Position.z - pos.z) < 1
					&& pos.y + 0.5 > camera->Position.y - 1.5
					&& pos.y - 0.5 < camera->Position.y + 0.5) {	// Ҫ�ŵķ��鲻��������ײ
					return;
				}
				// cout << "������:" << pos.x << " " << pos.y << " " << pos.z << endl;
				world->put_block(pos.x, pos.y, pos.z);
			}
		}
	}

}


void Game::render_cross()
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);  // ѡ��ͶӰ����
	glPushMatrix();               // ����ԭ����
	glLoadIdentity();             // װ�뵥λ����
	glOrtho(0, 100, 0, 100, -1, 1);    // λ����ͶӰ
	glMatrixMode(GL_MODELVIEW);   // ѡ��Modelview����
	glPushMatrix();               // ����ԭ����
	glLoadIdentity();             // װ�뵥λ����
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	glVertex2i(49, 50);
	glVertex2i(51, 50);
	glVertex2i(50, 49);
	glVertex2i(50, 51);
	glEnd();

	glMatrixMode(GL_PROJECTION);  // ѡ��ͶӰ����
	glPopMatrix();                // ����Ϊԭ�������
	glMatrixMode(GL_MODELVIEW);   // ѡ��Modelview����
	glPopMatrix();                // ����Ϊԭ�������
	glEnable(GL_DEPTH_TEST);
}