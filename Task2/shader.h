#pragma once

#include <glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;



class shader
{
private:
	unsigned int shaderprogram;
	void check(string str, int object) {

		int success;
		char info[520];

		if (str == "shader") {

			glGetShaderiv(object, GL_COMPILE_STATUS, &success);//iv ��ʲô��˼  ������״�� ���success = 0,����ʧ��
			if (!success) {

				glGetShaderInfoLog(object, 520, NULL, info);//������������ʲô��˼ �ڶ�����˵ָ���������ж�����˼��
				cout << "SHADER ERRORS: " << info << endl;

			}
		}

		if (str == "program") {

			glGetProgramiv(object, GL_LINK_STATUS, &success);
			if (!success) {

				glGetProgramInfoLog(object, 520, NULL, info);
				cout << "PROGRAM ERROR: " << info << endl;

			}
		}
	}



public:
	shader(const char *vPath, const char *fPath) {

		string vshadersource;
		string fshadersource;
		ifstream vP(vPath);
		ifstream fP(fPath);

		stringstream vStringObject;
		stringstream fStringObject;

		vStringObject << vP.rdbuf();
		fStringObject << fP.rdbuf();

		vP.close();
		fP.close();

		vshadersource = vStringObject.str();
		fshadersource = fStringObject.str();

		const char *vertexshader = vshadersource.c_str();
		const char *fragmentshader = fshadersource.c_str();

		GLint vertexShaderObject, fragmentShaderObject;
		vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
		fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vertexShaderObject, 1, &vertexshader, NULL);//���һ��������ʲô��˼ �ڶ�����������˼�� ������������˼��
		glCompileShader(vertexShaderObject);
		check("shader", vertexShaderObject);

		glShaderSource(fragmentShaderObject, 1, &fragmentshader, NULL);
		glCompileShader(fragmentShaderObject);
		check("shader", fragmentShaderObject);

		shaderprogram = glCreateProgram();
		glAttachShader(shaderprogram, vertexShaderObject);//�Ѷ�����ɫ����ϵ�����������
		glAttachShader(shaderprogram, fragmentShaderObject);//Ϊ��Ҫ��ϵ��һ�����������
		glLinkProgram(shaderprogram);
		check("program", shaderprogram);

		glDeleteShader(vertexShaderObject);
		glDeleteShader(fragmentShaderObject);


	}


	shader() {


	}



	//������ɫ���������
	void use() {

		glUseProgram(shaderprogram);
	}

	void setInt(const char *str, int value) {

		glUniform1i(glGetUniformLocation(shaderprogram, str), value);
	}

	void setMat4(const char *str, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderprogram, str), 1, GL_FALSE, &mat[0][0]);
	}

};