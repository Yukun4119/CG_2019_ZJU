#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.h"

/*
�������λ�ü��������ά�����е����ߣ���Ҫ�ο�
https://www.youtube.com/watch?v=DLKN0jExRIM
*/

struct IntersectPoint
{
	float x;
	float y;
	float z;
	bool isIntersect;
	float distance;
	glm::vec3 dir;

	IntersectPoint(float x, float y, float z, bool isIntersect, float distance, glm::vec3 dir) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->isIntersect = isIntersect;
		this->distance = distance;
		this->dir = dir;
	}

};

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

	// �жϵ�ǰ�������������Ǹ����ཻ
	// �������߷������λ�ã���λ��������������
	// ����
	glm::vec3 Intersect(glm::vec3 ray, glm::vec3 camera, int x, int y, int z);
};