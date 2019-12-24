#include "chunkManager.h"

ChunkManager::ChunkManager(Shader& shader)
{
	this->shader = shader;
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::init(Shader shader)
{
	this->shader = shader;
}

int ChunkManager::chunked(int x)
{
	return floor(float(x) / CHUNK_SIZE);
}

// ��ȡ��������(x, z)����ߵ��y����
int ChunkManager::highest(int x, int z)
{
	int X = chunked(x), Z = chunked(z);

	return getChunk(X, Z).highest(x-X*CHUNK_SIZE, z-Z*CHUNK_SIZE);
}

// ��ȡ��������(X, Z)������
Chunk& ChunkManager::getChunk(int X, int Z)
{
	auto it = chunks.begin();
	for (; it != chunks.end(); it++)
		if ((*it)->X == X && (*it)->Z == Z)
			return **it;
	
	// �����鲻���ڣ����ʼ��һ�����鲢����
	Chunk* chunk = new Chunk(X, Z);
	chunk->genChunk();
	chunks.push_back(chunk);
	return *chunk;
}

// �����������(X, Z)���Ƿ��������
bool ChunkManager::chunkExist(int X, int Z)
{
	auto it = chunks.begin();
	for (; it != chunks.end(); it++)
		if ((*it)->X == X && (*it)->Z == Z)
			return true;
	return false;
}

// ��ȡ��������(x, y, z)���ķ�������
BlockType ChunkManager::getBlock(int x, int y, int z)
{
	int X = chunked(x), Z = chunked(z);

	return getChunk(X, Z).getBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE);
}

// ��һ������Ϊtype�ķ���ŵ���������(x, y, z)��
void ChunkManager::putBlock(int x, int y, int z, BlockType type)
{
	int X = chunked(x), Z = chunked(z);

	getChunk(X, Z).putBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE, type);
}

// �Ƴ���������(x, y, z)���ķ���
void ChunkManager::removeBlock(int x, int y, int z)
{
	int X = chunked(x), Z = chunked(z);

	getChunk(X, Z).removeBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE);
}

// ʹ��render����camera��Χ������
void ChunkManager::render(glm::vec3 pos, glm::mat4 projViewMatrix)
{
	int cX = chunked(pos.x);
	int cZ = chunked(pos.z);

	// ɾ��λ��������Ĵݻٰ뾶�������
	for (auto it = chunks.begin(); it != chunks.end();) {
		if (std::max(abs((*it)->X-cX), abs((*it)->Z-cZ)) > DESTROY_RADIUS) {
			delete *it;
			it = chunks.erase(it);
		} else {
			it++;
		}
	}

	glEnable(GL_CULL_FACE);

	shader.Use();
	shader.SetInteger("tex", 0);
	shader.SetMatrix4("matrix", projViewMatrix);

	Texture2DArray textureArray = ResourceManager::GetTextureArray("blocks");
	textureArray.Bind();

	for (int i = cX-CREATE_RADIUS; i <= cX+CREATE_RADIUS; i++) {
		for (int j = cZ-CREATE_RADIUS; j <= cZ+CREATE_RADIUS; j++) {
			Chunk *c = &getChunk(i, j);

			if (!c->isLoaded()) 
				c->genChunk();

			if (c->isDirty())
				c->genBuffer();

			glBindVertexArray(c->vao);
			glDrawArrays(GL_TRIANGLES, 0, c->vertsNum);
		}
	}
}