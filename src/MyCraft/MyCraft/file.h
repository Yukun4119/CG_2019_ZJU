//txt�洢 xyztype

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "blockType.h"
#include "GL/glew.h"
#include<Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
using namespace std;

class File
{
public:
	ofstream outfile;//д��txt����
	fstream infile;//����txt����
	vector<GLint>vx;//��¼txt�ļ������������꼰blocktype
	vector<GLint>vy;
	vector<GLint>vz;
	vector<GLint>vtype;
	bool bRead = false;

	void Init();//����txt�ļ����ļ���������
	void CreatePath();

	void WriteCube(int x, int y, int z, BlockType type);//���ӷ���ʱд�뷽�����꼰blocktype
	void RemoveCube(int x, int y, int z);//ɾȥ����ʱд�뷽�����꣬BlockType��Ӧ��д��-1

private:
	string line;
	string filename = "";

	bool ExistTxt(string pathName);//�ж��Ƿ����txt
	void CreateFolder(string folderPath);//����save�ļ���
	void ReadFile();//��ȡ�ļ���������ɾ��������������Ӧ����
};