#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <iostream>

#include "../blockType.h"
#include "perlinNoise.h"

const int CHUNK_SIZE = 16;
const int CHUNK_HEIGHT = 128;

class Chunk
{
public:
	int X, Z;	// ����λ��

	Chunk(int x, int z) ;
	~Chunk();

	void genChunk();	// ���ɻ���������
	void genBuffer();	// ���ɲ���buffer
	void render();	

	int highest(int x, int z);	// ��ȡָ��λ�õ����߶�

	BlockType getBlock(int x, int y, int z);	// ��ȡָ��λ�÷���
	void putBlock(int x, int y, int z, BlockType w);	// ��ָ��λ�÷��÷���
	void removeBlock(int x, int y, int z);	// �Ƴ�ָ��λ�õķ���
private:
	bool loaded = false;	// �����жϷ����Ƿ������ɻ�����
	bool dirty = true;		// �����ж�buffer�Ƿ������ɲ���

	GLuint vao = 0, vbo = 0;
	int vertsNum = 0;
	std::unordered_map<int, BlockType> blocks;

	// xyzΪʵ�ʵ�xyz
	void genCubeBuffer(std::vector<float> &data, int x, int y, int z, BlockType w, 
		bool left, bool right, bool top, bool bottom, bool front, bool back);
};

