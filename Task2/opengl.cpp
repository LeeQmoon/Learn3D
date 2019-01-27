#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
#include"shader.h"
#include"Camera.h"

using namespace std;
const int width = 800;
const int height = 600;

GLboolean switchOne = false;//此开关控制出入光标窗口
GLboolean switchTwo = false;//此开关控制出入窗口后必须点击鼠标左键方可进行视角切换
GLboolean firstmouse = true;//此开关控制首次进入窗口设置光标，避免跳跃
GLdouble currenttime = 0.0f;
GLdouble lasttime = 0.0f;
GLdouble deltatime = 0.0f;
GLdouble lastX = width / 2.0f;
GLdouble lastY = height / 2.0f;


Camera camera;

void key_callback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods);
void mouse_Movement_callback(GLFWwindow *window, GLdouble xops, GLdouble yos);
void mouse_Scroll_callback(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset);
void mouse_Button_callback(GLFWwindow *window, GLint button, GLint action, GLint mods);
void cursor_Enter_callback(GLFWwindow *window, GLint enter);

GLfloat vertexF[] = {


	//右侧
	0.5f,0.5f,-0.5f,       1.0f,1.0f,
	0.5f,0.5f,0.5f,        0.0f,1.0f,
	0.5f,-0.5,0.5f,        0.0f,0.0f,
	0.5f,0.5f,-0.5f,       1.0f,1.0f,
	0.5f,-0.5f,0.5f,       0.0f,0.0f,
	0.5f,-0.5f,-0.5f,      1.0f,0.0f,

	//左侧
	-0.5f,0.5f,0.5f,       1.0f,1.0f,
	-0.5f,0.5f,-0.5f,      0.0f,1.0f,
	-0.5f,-0.5f,-0.5f,     0.0f,0.0f,
	-0.5f,0.5f,0.5f,       1.0f,1.0f,
	-0.5f,-0.5f,-0.5f,     0.0f,0.0f,
	-0.5f,-0.5f,0.5f,      1.0f,0.0f,


};




GLfloat vertexS[] = {
	//后面

	-0.5f,0.5f,-0.5f,    1.0f,1.0f,
	0.5f,0.5f,-0.5f,     0.0f,1.0f,
	-0.5f,-0.5f,-0.5f,   1.0f,0.0f,
	0.5f,0.5f,-0.5f,     0.0f,1.0f,
	0.5f,-0.5f,-0.5f,    0.0f,0.0f,
	-0.5f,-0.5f,-0.5f,   1.0f,0.0f,

	-0.5f,0.5f,0.5f,     0.0f,1.0f,
	-0.5f,-0.5f,0.5f,    0.0f,0.0f,
	0.5f,0.5f,0.5f,      1.0f,1.0f,
	0.5f,0.5f,0.5f,      1.0f,1.0f,
	-0.5f,-0.5f,0.5f,    0.0f,0.0f,
	0.5f,-0.5f,0.5f,     1.0f,0.0f,


};





int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow *window = glfwCreateWindow(width, height, "Test Something", NULL, NULL);
	if (window == NULL) {

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_Movement_callback);
	glfwSetScrollCallback(window, mouse_Scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_Button_callback);
	glfwSetCursorEnterCallback(window, cursor_Enter_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	unsigned int VBO[2], VAO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexS), vertexS, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	//纹理对象
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//缠绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);//取得图片相应数据  width height nrchannels
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//得到纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);//释放图片资源



	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexF), vertexF, GL_STATIC_DRAW);//这也就跟上面初始化数组时相对应，上面是[],未自行指定，
																			//由数据指定，即可使用sizeof运算符
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//第一个参数：属性index，与着色器中分配的一样
	//第二个：一个属性元素有几个凑成
	//第三个：组成元素的数据类型
	//第四个：是否标准化  
	//第五个：步长 是指某顶点属性第二次出现的位置相对于第一次出现的位置需要跨越的字节数
	//第六：offset 指某属性第一次出现的位置

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));//注意最后一个参数
	glEnableVertexAttribArray(1);


	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//缠绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);//取得图片相应数据  width height nrchannels
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//得到纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);//释放图片资源




	glBindVertexArray(0);
	shader myshader("vshader.txt", "fshaderOne.txt");
	myshader.use();


	shader mshader("vshader.txt", "fshaderTwo.txt");
	mshader.use();


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-0.6f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	glm::mat4 view;
	glm::mat4 projection;



	while (!glfwWindowShouldClose(window)) {


		currenttime = glfwGetTime();
		deltatime = currenttime - lasttime;
		lasttime = currenttime;

		glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);//最后两个参数不懂哦、

		cout << "model: " << endl;
		for (int i = 0; i < 4; i++) {

			for (int j = 0; j < 4; j++)
				cout << model[i][j] << " ";

			cout << endl;
		}

		cout << endl << endl;

		cout << "view: " << endl;
		for (int i = 0; i < 4; i++) {

			for (int j = 0; j < 4; j++)
				cout << view[i][j] << " ";

			cout << endl;
		}
		cout << endl << endl;


		cout << "projection: " << endl;
		for (int i = 0; i < 4; i++) {

			for (int j = 0; j < 4; j++)
				cout << view[i][j] << " ";

			cout << endl;
		}
		cout << endl << endl;




		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(VAO[0]);
		myshader.use();
		myshader.setMat4("view", view);
		myshader.setMat4("projection", projection);
		myshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 12);


		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO[1]);
		mshader.use();
		mshader.setMat4("view", view);
		mshader.setMat4("projection", projection);
		mshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 12);


		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	glfwTerminate();
	glDeleteBuffers(2, VBO);
	glDeleteVertexArrays(2, VAO);




	return 0;
}


void key_callback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods)
{

	if (switchOne&&switchTwo) {


		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwWindowShouldClose(window);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.Key_Movement(GLFW_KEY_W, deltatime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.Key_Movement(GLFW_KEY_S, deltatime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.Key_Movement(GLFW_KEY_A, deltatime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.Key_Movement(GLFW_KEY_D, deltatime);
	}

}


void mouse_Movement_callback(GLFWwindow *window, GLdouble xpos, GLdouble ypos)
{
	GLdouble xoffset, yoffset;

	if (firstmouse) {

		lastX = xpos;
		lastY = ypos;
		firstmouse = false;

	}


	if (switchOne&&switchTwo) {

		xoffset = xpos - lastX;
		yoffset = lastY - ypos;//注意此处  屏幕坐标的y轴自顶向下增大，而照相机坐标系的y轴自顶向下减小，二者刚好相反了	
		lastX = xpos;
		lastY = ypos;
		camera.Mouse_Movement(xoffset, yoffset);

	}
	/*
	或者：
	yoffset = ypos-lastY;
	lastY = -ypos;
	*/


}


void mouse_Scroll_callback(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset)
{
	if (switchOne&&switchTwo)
		camera.Cursor_Scoll(yoffset);
}

void mouse_Button_callback(GLFWwindow *window, GLint button, GLint action, GLint mods)
{

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		switchTwo = true;
		firstmouse = true;
	}

}

void cursor_Enter_callback(GLFWwindow *window, GLint enter)
{
	if (enter) {

		switchOne = true;
		firstmouse = true;
	}
	else {

		switchOne = false;
		switchTwo = false;
	}

}