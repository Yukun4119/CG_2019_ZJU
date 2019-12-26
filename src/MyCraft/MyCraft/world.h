#pragma once

#include "GL/glew.h"
#include "cubeRender.h"
#include "chunk/chunkManager.h"
#include "robot.h"
#include "skyBox.h"
#include "tree.h"

#include <vector>
#include<unordered_map>

class ChunkManager;
class Camera;

class World
{
private:
	CubeRender* cubeRender;
	TreeRender* treeRender;
	RobotRender* robotRender;

	bool picked = false;
	glm::vec3 pickedBlock;

	SkyBox skyBox;	// ��պж���
	std::vector<Chunk*> chunks;

	ChunkManager chunkManager;

	Chunk* findChunk(int x, int z);

	void drawWireCube(int x, int y, int z, glm::mat4 matrix);// �����߿�

	//�����������
	float mAmbientMaterial[4], mDiffuseMaterial[4], mSpecularMaterial[4];

public:
	World();
	~World();

	ChunkManager &getChunkManager();

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
	//void render(Camera& camera);
	void render(glm::mat4 matrix, glm::vec3 cameraPos);

	//�����������巴�����
	void SetAmbientMaterial(float r, float g, float b, float a);
	void SetDiffuseMaterial(float r, float g, float b, float a);
	void SetSpecularMaterial(float r, float g, float b, float a);
};