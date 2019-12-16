#pragma once

#include "cubeRender.h"
#include <vector>

class Block
{
protected:
	bool isSelected;


public:
	// �����������е�����, (��������ĵ�)
	std::vector<float> position;

	Block(float x, float y, float z);

	Block();

	// ���Ʒ��飬�麯����������������������
	virtual void Draw(CubeRender* Render) = 0;

	void select_block();
	void unselect_block();
};