#pragma once

#include "GL/glew.h"
#include "block.h"
#include "skyBox.h"
#include "cubeRender.h"
#include "blockFactory.h"
#include "chunk/chunk.h"

#include <vector>
#include<unordered_map>

const int WIDTH = 100;

class World
{
private:
	CubeRender* cubeRender;

	// ��պж���
	SkyBox skyBox;
	// Ҫ��Ⱦ������
	std::vector<Chunk*> chunks;

	int chunked(int x);

	//�����������
	float mAmbientMaterial[4], mDiffuseMaterial[4], mSpecularMaterial[4];

public:

	// ��һ�� Block ��ָ��������
	void put_block(int x, int y, int z, BlockType type);

	void clear_block(int x, int y, int z);

	// ʰȡ��block�� �Ὣ BLock ��isSelectֵ��Ϊtrue
	void pick_block(int x, int y, int z);
	// ����ȡ��ѡ��
	void unpick_block(int x, int y, int z);
	
	char get_map(int x, int y, int z);

	int highest(int x, int z);

	// ��chunk����Ѱ��chunk
	Chunk *findChunk(int x, int z);

	World();
	~World();

	// ���ļ��м�����������, ������Ӧ�ķ������
	void Load();

	// ��ʼ���������ɣ�׼����ɫ���������
	void init();

	// ��Ⱦ��������
	void render();

	//�����������巴�����
	void SetAmbientMaterial(float r, float g, float b, float a);
	void SetDiffuseMaterial(float r, float g, float b, float a);
	void SetSpecularMaterial(float r, float g, float b, float a);
};