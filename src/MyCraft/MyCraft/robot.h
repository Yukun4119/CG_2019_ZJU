#pragma once
#define N_ROBOT 4
#include "GL/glew.h"
#include "GL/glut.h"
#include <random>
#include <map>
#include <iostream>
#include <string>


class Robot {
public:
	float size;
	float speed;

	float x, y, z;
	int random_t;//����һ��������ߵĲ���
	unsigned int count;//��ǰ��ĳһ�����ߵĲ���
	float vangle;    // �ٶ�ƫ��
	bool move;

	// ���캯��
	Robot();
	Robot(int x, int y, int z, float s = 1);

	// ��������
	void setLocation(float x, float y, float z);
	void randomMove();

};



class RobotRender {
public:
	Robot robotList[N_ROBOT];
	GLint lid;
	GLint createList();
	void DrawRobot(Robot& robot);
	RobotRender();
};

