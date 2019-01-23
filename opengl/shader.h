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

			glGetShaderiv(object, GL_COMPILE_STATUS, &success);//iv 是什么意思  检查编译状况 如果success = 0,编译失败
			if (!success) {

				glGetShaderInfoLog(object, 520, NULL, info);//第三个参数是什么意思 第二个是说指定的数组有多大的意思吗
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
		glShaderSource(vertexShaderObject, 1, &vertexshader, NULL);//最后一个参数是什么意思 第二个参数的意思是 创建几个的意思吗
		glCompileShader(vertexShaderObject);
		check("shader", vertexShaderObject);

		glShaderSource(fragmentShaderObject, 1, &fragmentshader, NULL);
		glCompileShader(fragmentShaderObject);
		check("shader", fragmentShaderObject);

		shaderprogram = glCreateProgram();
		glAttachShader(shaderprogram, vertexShaderObject);//把顶点着色器联系到程序对象上
		glAttachShader(shaderprogram, fragmentShaderObject);//为何要联系到一个程序对象上
		glLinkProgram(shaderprogram);
		check("program", shaderprogram);

		glDeleteShader(vertexShaderObject);
		glDeleteShader(fragmentShaderObject);


	}


	shader() {


	}



	//激活着色器程序对象
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