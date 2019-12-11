#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.h"

/*
�������λ�ü��������ά�����е����ߣ���Ҫ�ο�
https://www.youtube.com/watch?v=DLKN0jExRIM
*/

class MousePicker
{
private:
	glm::vec3 currentRay;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	float mouseX, mouseY;
	float width, height;


	Camera* camera;

	glm::vec2 getNormalizedDeviceCoords();

	glm::vec4 toEyeCoords(glm::vec4 clipCoords);

	glm::vec3 toWorldCoords(glm::vec4 eyeCoords);

	glm::vec3 calculateMouseRay();

public:
	MousePicker(Camera* camera, glm::mat4 projection);

	glm::vec3 getCurrentRay();

	// ��Ҫ������ƶ���ʱ����øú�������
	void update(float mouseX, float mouseY, float width, float height);

	// �����������
	void render_ray();
};