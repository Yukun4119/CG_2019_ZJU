#pragma once

#include "chunk.h";
#include "../resource_manager.h"

const int CREATE_RADIUS = 4;
const int DESTROY_RADIUS = 6;

class ChunkManager
{
public:
	ChunkManager() {}
	ChunkManager(Shader& shader);
	~ChunkManager();

	void init(Shader shader);

	int chunked(int x);
	int highest(int x, int z);	// ��ȡ��������(x, z)����ߵ��y����

	Chunk& getChunk(int X, int Z);	// ��ȡ��������(X, Z)������ 
	bool chunkExist(int X, int Z);	// �����������(X, Z)���Ƿ��������

	BlockType getBlock(int x, int y, int z);	// ��ȡ��������(x, y, z)���ķ�������
	void putBlock(int x, int y, int z, BlockType type);	// ��һ������Ϊtype�ķ���ŵ���������(x, y, z)��
	void removeBlock(int x, int y, int z);	// �Ƴ���������(x, y, z)���ķ���

	void render(glm::vec3 pos, glm::mat4 projViewMatrix);

private:
	Shader shader;

	std::vector<Chunk*> chunks;
};

