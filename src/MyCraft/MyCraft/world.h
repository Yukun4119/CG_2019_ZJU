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

	bool picked = false;
	glm::vec3 pickedBlock;

	SkyBox skyBox;	// ��պж���
	std::vector<Chunk*> chunks;	// Ҫ��Ⱦ������

	int chunked(int x);
	Chunk *findChunk(int x, int z);	// ��chunk����Ѱ��chunk

	void drawWireCube(int x, int y, int z, glm::mat4 matrix);// �����߿�

	//�����������
	float mAmbientMaterial[4], mDiffuseMaterial[4], mSpecularMaterial[4];

public:
	World();
	~World();

	// �޸���Ҫ���Ƶ��߿�λ��
	void pick_block(int x, int y, int z);
	void unpick_block();

	BlockType get_block(int x, int y, int z);

	// ��һ�� Block ��ָ��������
	void put_block(int x, int y, int z, BlockType type);

	void remove_block(int x, int y, int z);

	int highest(int x, int z);

	// ���ļ��м�����������, ������Ӧ�ķ������
	void Load();

	// ��ʼ���������ɣ�׼����ɫ���������
	void init();

	// ��Ⱦ��������
	void render(glm::mat4 matrix, glm::vec3 cameraPos);

	//�����������巴�����
	void SetAmbientMaterial(float r, float g, float b, float a);
	void SetDiffuseMaterial(float r, float g, float b, float a);
	void SetSpecularMaterial(float r, float g, float b, float a);
};