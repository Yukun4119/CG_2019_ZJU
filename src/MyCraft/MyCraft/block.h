#pragma once

#include "cubeRender.h"
#include <vector>

class Block
{
private:
	bool isSelected;


public:
	// �����������е�����, (��������ĵ�)
	std::vector<float> position;

	Block(float x, float y, float z);

	Block();

	// ���Ʒ���
	void Draw(CubeRender* Render);

	void select_block();
	void unselect_block();
};