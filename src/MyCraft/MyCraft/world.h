#pragma once

#include "GL/glew.h"
#include "block.h"
#include "skyBox.h"
#include "cubeRender.h"

#include <vector>
#include<unordered_map>

const int WIDTH = 100;

class World
{
private:
	CubeRender* cubeRender;

	// ��պж���
	SkyBox skyBox;

	void set_map(int x, int y, int z);

	char map[WIDTH][WIDTH][WIDTH];

	// Ҫ��Ⱦ�ķ���
	// std::vector<Block> blocks;
	// ������ map �洢
	std::unordered_map<int, Block> blockMap;

	// �����������꣬�����Ӧ�� map �еļ�ֵ
	int get_block_id(int x, int y, int z);

public:

	// ��һ�� Block ��ָ��������
	void put_block(int x, int y, int z);

	// ʰȡ��block�� �Ὣ BLock ��isSelectֵ��Ϊtrue
	void pick_block(int x, int y, int z);
	// ����ȡ��ѡ��
	void unpick_block(int x, int y, int z);
	
	char get_map(int x, int y, int z);

	World();
	~World();

	// ���ļ��м�����������, ������Ӧ�ķ������
	void Load();

	// ��ʼ���������ɣ�׼����ɫ���������
	void init();

	// ��Ⱦ��������
	void render();
};