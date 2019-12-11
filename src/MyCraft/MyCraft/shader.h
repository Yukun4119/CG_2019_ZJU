#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h" // ����glew����ȡ���еı���OpenGLͷ�ļ�
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// ����ID
	GLuint ID;

	// ���ǰ��ɫ��
	Shader& Use();

	// Compiles the shader from given source code
	void    Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr); // Note: geometry source code is optional 
	
	void    SetFloat(const GLchar* name, GLfloat value, GLboolean useShader = false);
	void    SetInteger(const GLchar* name, GLint value, GLboolean useShader = false);
	void    SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void    SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader = false);
	void    SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void    SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader = false);
	void    SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void    SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader = false);
	void    SetMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader = false);
private:
	// Checks if compilation or linking failed and if so, print the error logs
	void    checkCompileErrors(GLuint object, std::string type);
};


#endif

/*

void init_shader()
{
	// ��������(��׼���豸����)���Ὣ�䷢�͸�������ɫ��
	GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	const GLchar* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"}\0";
	const GLchar* fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	// ʹ�� GLSL ���Ա�д������ɫ��
	// �����ڶ�̬����ʱ��������ɫ������
	// �ȴ���һ����ɫ������
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// ����ɫ��Դ�븽�ŵ���ɫ��������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// ������ʱ��Ĵ���
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ��дƬ����ɫ��
	// ����Ƭ����ɫ��
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ������ɫ������ú󣬽���������ɫ���������ӵ�������Ⱦ����ɫ��������
	//	��ɫ������
	//	��ɫ���������(Shader Program Object)�Ƕ����ɫ���ϲ�֮������������ɵİ汾��
	//	Ҫʹ�ñ���õ���ɫ������Ҫ����Ϊ��ɫ��������󣬲�����Ⱦ�����ʱ�򼤻���ɫ������
	//	�Ѽ�����ɫ���������ɫ���������Ƿ�����Ⱦ���õ�ʱ��ʹ�á�

	// ����һ����ɫ���������
	shaderProgram = glCreateProgram();
	// ������ɫ�����󣺵�������ɫ����һ�������ʱ�������ÿ����ɫ����������ӵ��¸���ɫ�������롣
	// ����������벻ƥ���ʱ�����õ�һ�����Ӵ���
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// ����ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ����
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// ������ɫ������GPU�д����ڴ����ڴ洢��������
	// ͨ�����㻺�����VBO ����������ڴ棬������GPU�ڴ�(ͨ������Ϊ�Դ�)�д���������㡣
	// ��������ķ�������һ�� VBO ����
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	// OpenGL�кܶ໺��������ͣ����㻺�����Ļ���������GL_ARRAY_BUFFER
	// ����ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// ����glBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
	// ע��󶨻������ͺ�ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ������OpenGL����ν��Ͷ�������
	// ���Ӷ�������: ָ���������ݵ���һ�����ֶ�Ӧ������ɫ������һ����������(���������ڶ�����ɫ����������)
	// ʹ��glVertexAttribPointer��������OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
	glVertexAttribPointer(
		0, // ָ��Ҫ���õĶ�������, �����ڶ�����ɫ������ָ�� layout (location = 0)
		3, // �������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3
		GL_FLOAT, // �������ͣ�(GLSL��vec*�����ɸ�����ֵ��ɵ�)
		GL_FALSE, // �Ƿ�ϣ�����ݱ���׼����������������ݶ��ᱻӳ�䵽0-1֮��
		3 * sizeof(GLfloat), // �����Ķ���������֮��ļ����
		(GLvoid*)0	// ����ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0
	);
	
	// ÿ���������Դ�һ��VBO������ڴ��л���������ݣ�
	// �������Ǵ��ĸ�VBO�������п����ж��VBO����ȡ����ͨ����
	// ����glVetexAttribPointerʱ�󶨵�GL_ARRAY_BUFFER��VBO�����ġ�
	// �����ڵ���glVetexAttribPointer֮ǰ�󶨵�����ǰ�����VBO����
	// ��������0���ڻ����ӵ����Ķ������ݡ�
	// �Զ�������λ��ֵ��Ϊ���������ö�������
	glEnableVertexAttribArray(0);

	// Note that this is allowed, the call to glVertexAttribPointer registered VBO 
	// as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	glBindVertexArray(0);
	
}
*/

/*
	�� OpenGL ������������������������:
	// 0. ���ƶ������鵽�����й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 1. ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 2. ��������Ⱦһ������ʱҪʹ����ɫ������
	glUseProgram(shaderProgram);
	// 3. ��������
	someOpenGLFunctionThatDrawsOurTriangle();

	ʹ�ö������������԰����е�״̬���ô�����һ�������У����ҿ���ͨ��������������ָ�״̬

	�����������(Vertex Array Object, VAO)�����񶥵㻺������������󶨣�
	�κ����Ķ������Ե��ö��ᴢ�������VAO�С������ĺô����ǣ�
	�����ö�������ָ��ʱ����ֻ��Ҫ����Щ����ִ��һ�Σ�֮���ٻ��������ʱ��
	ֻ��Ҫ����Ӧ��VAO�����ˡ���ʹ�ڲ�ͬ�������ݺ���������֮���л����
	�ǳ��򵥣�ֻ��Ҫ�󶨲�ͬ��VAO�����ˡ��ո����õ�����״̬�����洢��VAO��


	����һ��VAO�ʹ���һ��VBO�����ƣ�
		GLuint VAO;
		glGenVertexArrays(1, &VAO);
	Ҫ��ʹ��VAO��Ҫ����ֻ��ʹ��glBindVertexArray��VAO��
	�Ӱ�֮��������Ӧ�ð󶨺����ö�Ӧ��VBO������ָ�룬
	֮����VAO��֮��ʹ�á������Ǵ������һ�������ʱ��
	����ֻҪ�ڻ�������ǰ�򵥵ذ�VAO�󶨵�ϣ��ʹ�õ��趨�Ͼ����ˡ�
	��δ���Ӧ�ÿ�������������

		// ..:: ��ʼ�����루ֻ����һ�� (�����������Ƶ���ı�)�� :: ..
		// 1. ��VAO
		glBindVertexArray(VAO);
		// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 3. ���ö�������ָ��
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//4. ���VAO
		glBindVertexArray(0);

		[...]

		// ..:: ���ƴ�����Ϸѭ���У� :: ..
		// 5. ��������
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		someOpenGLFunctionThatDrawsOurTriangle();
		glBindVertexArray(0);
	*/