// �� fopen ���ᱨ��,������ C/C++, Ԥ��������Ԥ������������ ��� _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#define GLEW_STATIC
#include "GL/glew.h"

#include "game.h"

// The Width of the screen
const GLuint SCREEN_WIDTH = 1000;
// The height of the screen
const GLuint SCREEN_HEIGHT = 800;

Game MineCraft(SCREEN_WIDTH, SCREEN_HEIGHT);

// the screen size
int screenWidth, screenHeight;

void reshape(int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	glViewport(0, 0, w, h);
}

void timer(int extra)
{
	// �����û�������
	MineCraft.ProcessInput(0.01);
	// ������Ϸ
	MineCraft.Update(0.01);

	// �ػ滭��
	glutPostRedisplay();
	// ÿ 5ms ����һ�θú����� ��Ҫ���ڿ���֡��fps
	glutTimerFunc(5, timer, 0);
}

void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf_s(buffer, "FPS:%4.2f ", frame * 1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}

	char* c;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);  // ѡ��ͶӰ����
	glPushMatrix();               // ����ԭ����
	glLoadIdentity();             // װ�뵥λ����
	glOrtho(0, 480, 0, 480, -1, 1);    // λ����ͶӰ
	glMatrixMode(GL_MODELVIEW);   // ѡ��Modelview����
	glPushMatrix();               // ����ԭ����
	glLoadIdentity();             // װ�뵥λ����
	glRasterPos2f(10, 10);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);  // ѡ��ͶӰ����
	glPopMatrix();                // ����Ϊԭ�������
	glMatrixMode(GL_MODELVIEW);   // ѡ��Modelview����
	glPopMatrix();                // ����Ϊԭ�������
	glEnable(GL_DEPTH_TEST);
}

void display(void)
{
	// ��Ⱦ��Ϸ����
	MineCraft.Render();
	// ��ʾ FPS ��Ϣ
	getFPS();
	// ����������
	glutSwapBuffers();
}

void keyDown(unsigned char key, int x, int y)
{
	if (key == 27)	// ESC
	{
		exit(0);
	}
	if (key == 'f')
	{
		MineCraft.camera->flying = !MineCraft.camera->flying;
	}
	// ��¼��Ӧ�������µ�״̬
	if (key >= 0 && key < 1024)
	{
		MineCraft.Keys[key] = GL_TRUE;
	}
}

void keyUp(unsigned char key, int x, int y)
{
	if (key >= 0 && key < 1024)
	{
		MineCraft.Keys[key] = GL_FALSE;
	}
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// setting the clear color, seems like background
	glShadeModel(GL_SMOOTH);	// ��ɫ��ģʽ
	glEnable(GL_TEXTURE_2D);	// ������ӳ��

	// ��ʼ����Ϸ
	MineCraft.Init();

	timer(0);
}

void mouse_callback(int xpos, int ypos)
{
	MineCraft.MouseMoveCallback(xpos, ypos);
}

void  mouse_click_callback(int button, int state, int x, int y)
{
	MineCraft.MouseClickCallback(button, state, x, y);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Minecraft");
	glutSetCursor(GLUT_CURSOR_NONE);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

	init();

	// ���������ݻ��ƣ����ڴ�С�ı䣬�����ػ�ʱ�������display����
	glutDisplayFunc(display);

	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(mouse_callback);
	glutMouseFunc(mouse_click_callback);
	glutMainLoop();

	return 0;
}


