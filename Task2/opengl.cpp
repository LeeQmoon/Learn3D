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
GLboolean firstmouse = true;//此开关控制首次进入窗口设置光标，避免跳跃
GLdouble lastX = width / 2.0f;
GLdouble lastY = height / 2.0f;


Camera camera;

void key_callback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods);
void mouse_Movement_callback(GLFWwindow *window, GLdouble xops, GLdouble yos);
void mouse_Scroll_callback(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset);
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

	long int count = 0;


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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);//第一个参数：属性index，与着色器中分配的一样
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
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);//最后两个参数不懂哦、
	
	
	while (!glfwWindowShouldClose(window)) {
		// (0，0，1）
		// (0，0，0）
		// 速度-0.01 m/s
		// p = v * t


		glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Key_Movement();
		
		glm::mat4 view;
		view = camera.GetViewMatrix();


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

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		//设置按下/释放键为true或false
		if (action == GLFW_PRESS)
			camera.key_status[key] = true;
		else if (action == GLFW_RELEASE)
			camera.key_status[key] = false;
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


	if (switchOne) {

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
		camera.Cursor_Scoll(yoffset);
}


void cursor_Enter_callback(GLFWwindow *window, GLint enter)
{
	if (enter) {

		switchOne = true;
		firstmouse = true;
	}
	else {

		switchOne = false;
	
	}

}