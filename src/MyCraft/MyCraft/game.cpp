#include "game.h"
#include "resource_manager.h"

Game::Game(GLuint width, GLuint height)
{
	this->Width = width;
	this->Height = height;

	camera = nullptr;

	world = nullptr;

	mousePicker = nullptr;

	currentType = GRASS;

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

void Game::nextBlcokType()
{
	this->currentType = (BlockType)((this->currentType + 1) % BlockTypeNum);
	cout << this->currentType << endl;
}


void Game::Init()
{
	// ��������
	vector<const GLchar*> files;
	files.push_back("textures/grass_block_up.png");
	files.push_back("textures/grass_block_side.png");
	files.push_back("textures/ice.png");
	files.push_back("textures/gold_ore.png");
	ResourceManager::LoadTextureArray(files, false, "blocks");

	//װ����ľ����
	ResourceManager::LoadTexture("textures/tree.png", false, "trunk");
	ResourceManager::LoadTexture("textures/leaf.png", false, "leaves");

	ResourceManager::LoadShader("shaders/sky_vertex.glsl", "shaders/sky_fragment.glsl", nullptr, "shader_skybox");
	ResourceManager::LoadShader("shaders/chunk_vertex.glsl", "shaders/chunk_fragment.glsl", 
		nullptr, "shader_chunk");
	ResourceManager::LoadShader("shaders/line_vertex.glsl", "shaders/line_fragment.glsl",
		nullptr, "shader_line");

	// �½�һ�� world ����
	this->world = new World();
	// ����������Դ
	this->world->Load();
	// ��ʼ������
	this->world->init();

	// ����һ�������
	this->camera = new Camera(glm::vec3(3.0f, this->world->highest(3, 3)+2.5f, 3.0f));

	// �������ʰȡ��
	// this->mousePicker = new MousePicker(this->camera, glm::make_mat4(projectionMatrix));
	this->mousePicker = new MousePicker(this->camera, glm::perspective(65.0f, (float)Width / Height, 0.125f, 100.0f));

}

void Game::ProcessInput(GLfloat dt)
{
	camera->doMovement(this->Keys, world, dt);
	camera->doZoom(this->Keys, dt);
}


void Game::Render()
{
	// clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camera->getZooom(),(float)Width/Height,0.125f,100.0f);

	//����ģ�;���
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//�����ӵ�
	camera->setLookAt();

	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Width / Height, 0.125f, 100.0f);
	glm::mat4 matrix = projection * view;

	// ��Ⱦ����
	world->render(matrix, camera->Position);

	// �����������
	this->mousePicker->render_ray();

	// ��Ⱦ��Ļ���ĵ�ʮ��
	this->render_cross();

}

void Game::Update(GLfloat dt)
{
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
		if (world->get_block(x, y, z) != AIR) {
			if (block_last_x == x && block_last_y == y && block_last_z == z) {
				// cout << "�Ѿ�ѡ����"<< endl;
			}
			else {
				world->unpick_block();
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

			if (world->get_block(pos.x, pos.y, pos.z) == AIR) {	// ��ǰλ��û�з���
				if (glm::abs(camera->Position.x - pos.x) < 0.6
					&& glm::abs(camera->Position.z - pos.z) < 0.6
					&& pos.y + 0.3 > camera->Position.y - 1.3
					&& pos.y - 0.3 < camera->Position.y + 0.3) {	// Ҫ�ŵķ��鲻��������ײ
					return;
				}
				// cout << "������:" << pos.x << " " << pos.y << " " << pos.z << endl;
				world->put_block(pos.x, pos.y, pos.z, currentType);
				file.WriteCube(pos.x, pos.y, pos.z, currentType);
			}
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {		// ��������
		if (state == GLUT_DOWN) {
			if (world->get_block(block_last_x, block_last_y, block_last_z) != AIR) {
				world->remove_block(block_last_x, block_last_y, block_last_z);
				file.RemoveCube(block_last_x, block_last_y, block_last_z);
			}
		}
	}

}

void Game::Reshape(int w, int h)
{
	Width = w;
	Height = h;
	camera->setProjectionMatrix(glm::perspective(glm::radians(45.0f), (float)w/h, 0.125f, 100.0f));
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