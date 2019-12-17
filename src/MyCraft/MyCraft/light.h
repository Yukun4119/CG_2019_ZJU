#pragma once
#include "GL/glew.h"
#include<stdio.h>
#include "game.h"

//��֪��������ʲô�Ļ���pointlight��ע��

class Light {
protected:
	GLenum mLightIdentifier;
	Light();
public:
	void SetAmbientColor(float r, float g, float b, float a);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetSpecularColor(float r, float g, float b, float a);
	void Enable();
};

class DirectionLight :public Light {
public:
	DirectionLight(GLenum light);
	void SetPosition(float x, float y, float z);
};

class PointLight :public Light {
public:
	PointLight(GLenum light);//����ʲô��Դ
	void SetPosition(float x, float y, float z);//��Դλ��
	void SetConstAttenuation(float v);//����˥��ϵ��
	void SetLinearAttenuation(float v);//����˥������
	void SetQuadricAttenuation(float v);//ƽ��˥������
};

class SpotLight :public PointLight {
public:
	SpotLight(GLenum light);
	void SetDirection(float x, float y, float z);
	void SetExponent(float v);
	void SetCutoff(float v);
};

