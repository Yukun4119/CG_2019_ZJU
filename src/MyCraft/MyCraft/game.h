#pragma once
#include "GL/glew.h"
#include "camera.h"
#include "world.h"
#include "mousePicker.h"

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};


class Game
{
private:
	BlockType currentType;	// ��ǰ�����õķ�������

public:
	// ��Ϸ״̬
	GameState  State;
	GLboolean  Keys[1024];
	// ��Ϸ���ڵĿ��
	GLuint     Width, Height;

	Camera* camera;

	MousePicker* mousePicker;

	World* world;

	void nextBlcokType();

	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	
	// ������Դ(��ɫ��, ����), ��ʼ����Ϸ״̬
	void Init();

	// ʹ�ô洢�� keys �����е������������û�����
	void ProcessInput(GLfloat dt);

	void MouseMoveCallback(int x, int y);

	void MouseClickCallback(int button, int state, int x, int y);

	// ���ڸı�ʱ���������״̬
	void Reshape(int w, int h);

	// �ڸú����и�����Ϸ״̬����,���綯����Լ��ƶ�
	void Update(GLfloat dt);

	// ��Ⱦ��Ϸ
	void Render();

private:

	void render_cross();

};