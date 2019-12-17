#include "chunk.h"

Chunk::Chunk(int x, int z)
{
	X = x;
	Z = z;
}

Chunk::~Chunk()
{
	if (vao != 0)
		glDeleteVertexArrays(1, &vao);
	if (vbo != 0)
		glDeleteBuffers(1, &vbo);
}

// ���ɻ���������
void Chunk::genChunk()
{
	PerlinNoise noise;

	for (int x = 0; x < CHUNK_SIZE; x++)
		for (int z = 0; z < CHUNK_SIZE; z++) {
			// �����ʵ��λ��
			int xi = X * CHUNK_SIZE + x;
			int zi = Z * CHUNK_SIZE + z;

			// ����ʵ��λ�ü������߶�
			double f = noise.octaveNoise(xi*0.01, zi*0.01, 4, 0.5);
			double g = noise.octaveNoise(-xi*0.01, -zi*0.01, 2, 0.9);
			int mh = g * 32 + 16;
			int h = f * mh;
			int t = 12;
			if (h <= t) {
				h = t;
			}

			// ����Ŀǰ����Ч�ʹ��ͣ�ֻ��������
			for (int y = h-2; y < h; y++) {
				int key = (y * CHUNK_SIZE + x) * CHUNK_SIZE + z;
				blocks.insert(std::make_pair(key, GRASS));
			}
		}

	loaded = true;
}

// ���ɲ���buffer
void Chunk::genBuffer()
{
	if (!loaded)
		return;

	std::vector<float> data;

	// �������з���
	for (auto it = blocks.begin(); it != blocks.end(); it++) {
		if (it->second == AIR) continue;

		// �õ�����
		int key = it->first;
		int z = key % CHUNK_SIZE;
		key /= CHUNK_SIZE;
		int x = key % CHUNK_SIZE;
		int y = key / CHUNK_SIZE;

		bool left, right, top, bottom, front, back;
		left = right = top = bottom = front = back = false;
		if (getBlock(x-1, y, z) == AIR) left = true;
		if (getBlock(x+1, y, z) == AIR) right = true;
		if (getBlock(x, y+1, z) == AIR) top = true;
		if (getBlock(x, y-1, z) == AIR) bottom = true;
		if (getBlock(x, y, z+1) == AIR) front = true;
		if (getBlock(x, y, z-1) == AIR) back = true;
		
		genCubeBuffer(data, x+X*CHUNK_SIZE, y, z+Z*CHUNK_SIZE, it->second,
			left, right, top, bottom, front, back);
	}

	// ������
	vertsNum = 0;
	if (data.size() != 0)
		vertsNum = data.size() / 6;

	// ɾ�����е�vao��vbo
	if (vao != 0)
		glDeleteVertexArrays(1, &vao);
	if (vbo != 0)
		glDeleteBuffers(1, &vbo);

	// ���ɲ���vao��vbo
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), &data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void *)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void *)(3*sizeof(float)));

	dirty = false;
}

void Chunk::render()
{
	if (!loaded)
		genChunk();

	if (dirty)
		genBuffer();

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertsNum);
}

// ��ȡָ��λ�õ����߶�
int Chunk::highest(int x, int z)
{
	int highest = 0;

	for (auto it = blocks.begin(); it != blocks.end(); it++) {
		// �õ�����
		int key = it->first;
		int iz = key % CHUNK_SIZE;
		key /= CHUNK_SIZE;
		int ix = key % CHUNK_SIZE;
		int iy = key / CHUNK_SIZE;

		if (ix == x && iz == z) {
			highest = std::max(highest, iy);
		}
	}
	
	return highest;
}

// ��ȡָ��λ�õķ���
BlockType Chunk::getBlock(int x, int y, int z)
{
	// �ж��Ƿ�Խ��
	if (x > CHUNK_SIZE-1 || y > CHUNK_HEIGHT-1 || z > CHUNK_SIZE-1)
		return AIR;
	if (x < 0 || y < 0 || z < 0)
		return AIR;

	int key = (y * CHUNK_SIZE + x) * CHUNK_SIZE + z;
	auto it = blocks.find(key);
	if (it != blocks.end()) 
		return it->second;
	else
		return AIR;
}

// ��ָ��λ�÷��÷���
void Chunk::putBlock(int x, int y, int z, BlockType w)
{
	// �ж��Ƿ�Խ��
	if (x > CHUNK_SIZE-1 || y > CHUNK_HEIGHT-1 || z > CHUNK_SIZE-1)
		return;
	if (x < 0 || y < 0 || z < 0)
		return;

	int key = (y * CHUNK_SIZE + x) * CHUNK_SIZE + z;
	auto it = blocks.find(key);
	if (it != blocks.end()) 
		it->second = w;
	else
		blocks.insert(std::make_pair(key, w));

	dirty = true;
}

// �Ƴ�ָ��λ�õķ���
void Chunk::removeBlock(int x, int y, int z)
{
	// �ж��Ƿ�Խ��
	if (x > CHUNK_SIZE-1 || y > CHUNK_HEIGHT-1 || z > CHUNK_SIZE-1)
		return;
	if (x < 0 || y < 0 || z < 0)
		return;

	int key = (y * CHUNK_SIZE + x) * CHUNK_SIZE + z;
	auto it = blocks.find(key);
	if (it != blocks.end()) 
		blocks.erase(it);

	dirty = true;
}

// xyzΪʵ�ʵ�xyz
void Chunk::genCubeBuffer(std::vector<float>& data, int x, int y, int z, BlockType w, bool left, bool right, bool top, bool bottom, bool front, bool back)
{
	static const float position[6][6][5] = {
		// left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		// right
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

		 // top
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		
		 // bottom
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		// front
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

		// back
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	};
	

	bool faces[] = {left, right, top, bottom, front, back};

	// ���������6����
	for (int i = 0; i < 6; i++) {
		if (!faces[i])
			continue;

		// �������ϵ�6������
		for (int v = 0; v < 6; v++) {
			// xyz
			data.push_back(x+position[i][v][0]);
			data.push_back(y+position[i][v][1]);
			data.push_back(z+position[i][v][2]);
			// uv
			data.push_back(position[i][v][3]);
			data.push_back(position[i][v][4]);
			// w
			data.push_back(ETextureType::blockTextures[w][i]);
		}
	}
}
