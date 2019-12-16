#pragma once

#include "GL/glew.h"
#include "texture.h"
#include "shader.h"
#include "blockType.h"
#include <string>
#include <map>

// һ����Ⱦ��, ������Ⱦһ��������� 1x1x1 ��������
// �������ͨ�����ø���Ⱦ������Ⱦ


// TODO: �޸�Ϊ��ɫ����ʽ��Ⱦ
// https://learnopengl-cn.github.io/06%20In%20Practice/2D-Game/03%20Rendering%20Sprites/

class CubeRender
{
private:
	Shader shader;

	// ʹ����ʾ�б�ӿ���Ⱦ�ٶ�
	std::map<std::string, GLint> renderList;

	GLint createList(Texture2D tex_up, Texture2D tex_side, Texture2D tex_down);

	void drawWireCube(float x);

public:

	CubeRender();
	CubeRender(Shader& shader);

	// ����ɫ����ʱ���������� VAO �� VBO ��
	void initRenderData();

	// render to the screen
	void drawCube(float x, float y, float z, Texture2D tex_up, Texture2D tex_side, Texture2D tex_down);

	void drawWireCube(float x, float y, float z);
};