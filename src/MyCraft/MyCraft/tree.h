#pragma once
#include <vector>
#include"shader.h"
#include <string>
#include <map>
#include "GL/glew.h"
#include "texture.h"
#include "resource_manager.h"

using namespace std;
#define N_TREE (10)
//#define SIZE (16)
class Tree {
public:
	float position[3];//��ľλ��
	float leaf[2][2];//������Ҷ{r,h}
	float trunk[2];//����{r,h}
	Tree(float x, float y, float z, float r0 = 0.8, float h0 = 0.8, float r1 = 1, float h1 = 1, float r = 0.2, float h = 1.5);
};


class TreeRender {
private:	
	Shader shader;//TODO:ʹ����ɫ������
	map<std::string, GLint> renderList;//��ʾ�б�
	void drawLeaf(int level, Tree& tree, Texture2D& trunk, Texture2D& leaves);//������Ҷ
	void drawTrunk(Tree& tree, Texture2D& trunk, Texture2D& leaves);//����������
    // ʹ����ʾ�б�ӿ���Ⱦ�ٶ�
	GLint createList(Tree& tree, Texture2D& trunk, Texture2D& leaves);
	
public:
	int treelist[N_TREE][2]; //����λ�� ��x,z��
	TreeRender();
	TreeRender(Shader& shader);//TODO ����ɫ��������ľ
	
	void initRenderData();//TODO ����ɫ����ʱ���������� VAO �� VBO ��
	void DrawTree(Tree& tree, Texture2D& trunk, Texture2D& leaves);// ������ľ
    
};

