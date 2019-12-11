#include "world.h"
#include "resource_manager.h"


World::World()
{
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < WIDTH; j++) {
			for (int k = 0; k < WIDTH; k++) {
				map[i][j][k] = 0;
			}
		}
	}
}

char World::get_map(int x, int y, int z)
{
	if (x < -WIDTH / 2 || x > WIDTH / 2 || y < -WIDTH / 2 || y > WIDTH / 2 || z < -WIDTH / 2 || z > WIDTH / 2)
		return 0;
	return this->map[x + WIDTH / 2][y + WIDTH / 2][z + WIDTH / 2];
}

void World::set_map(int x, int y, int z)
{
	map[x + WIDTH / 2][y + WIDTH / 2][z + WIDTH / 2] = 1;
}


World::~World()
{
	// �ͷ���Ⱦ��
	if (cubeRender != nullptr)
		delete cubeRender;
}

void World::Load()
{
	// ��ʱ��������
	for (int i = -5; i < 5; i++)
	{
		for (int j = -5; j < 5; j++)
		{
			this->put_block(i, 0, j);
		}
	}

	this->put_block(0, 1, 0);	// �м��Ǹ�����

	this->put_block(-5, 1, -5);	// ����һȦ
	this->put_block(-5, 1, -4);
	this->put_block(-5, 1, -3);
	this->put_block(-5, 1, -2);
	this->put_block(-5, 1, -1);
	this->put_block(-5, 1, -0);
	this->put_block(-5, 1, 1);	// ����һȦ
	this->put_block(-5, 1, 2);
	this->put_block(-5, 1, 3);
	this->put_block(-5, 1, 4);
	this->put_block(-5, 1, 5);

	this->skyBox = SkyBox();
}

int World::get_block_id(int x, int y, int z)
{
	// ��ʱ���������Ƿ�����?
	return y * WIDTH * WIDTH + x * WIDTH + z;
}

void World::pick_block(int x, int y, int z)
{
	int id = get_block_id(x, y, z);

	std::unordered_map<int, Block>::iterator it;

	it = this->blockMap.find(id);

	if (it != this->blockMap.end())
	{
		it->second.select_block();
	}
}

void World::unpick_block(int x, int y, int z)
{
	int id = get_block_id(x, y, z);
	std::unordered_map<int, Block>::iterator it;

	it = this->blockMap.find(id);

	if (it != this->blockMap.end())
	{
		it->second.unselect_block();
	}
}


void World::put_block(int x, int y, int z)
{
	set_map(x, y, z);
	Block block(x, y, z);
	
	// ע��ӳ�䷽ʽ, id �Ƿ�����
	int id = get_block_id(x, y, z);
	blockMap.insert(std::pair<int, Block>(id, block));
}

void World::init()
{
	// ��ʼ����������Ⱦ��
	cubeRender = new CubeRender();

	// ��ʼ����պ�
	skyBox.init(ResourceManager::GetShader("shader_skybox"));
}

void World::render()
{
	// ��Ⱦ��պ�
	skyBox.render();

	// ���ü򵥵Ĺ���
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat gray[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat light_pos[] = { 10, 10, 10, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gray);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
	glEnable(GL_LIGHT0);

	// ʹ�� cubeRender ��Ⱦ���еķ������
	/*
		for (Block& block : this->blocks)
	{
		block.Draw(cubeRender);
	}
	*/
	for (auto& block : this->blockMap)
	{
		block.second.Draw(cubeRender);
	}


	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
}