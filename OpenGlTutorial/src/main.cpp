#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include<fstream>
#include<sstream>
#include<streambuf>
#include<string>
#include "graphics/shader.h"
#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/camera.h"
#include "io/screen.h"
#include "graphics/texture.h"
#include "graphics/models/cube.hpp"
#include "graphics/models/lamp.hpp"
#include "graphics/models/cube.hpp"
#include "graphics/models/gun.hpp"
#include "graphics/light.h"
#include "graphics/model.h"


using namespace std;

void processInput(double deltaTime);
//string loadShaderSrc(const char* filename);//to load the shaders files, i.e, vertex and fragments

unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
float x, y, z;

float mixVal = 0.5;
float theta = 45.f;
glm::mat4 mouseTransform = glm::mat4(1.0f);

Camera camera[2] = {
Camera(glm::vec3(0.0f, 0.0, 0.0f)),
Camera(glm::vec3(0.0f, 10.0, 10.0f))
};

Camera Camera::defaultCamera(glm::vec3(0.0f, 0.0f, 0.0f));
unsigned int active = 0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Screen screen;

bool flashLight = true;

int main() {
	cout << "Hello World" << endl;

	int success;
	char infoLog[512];

	//initialize glfw
	glfwInit();

	//open GLFW version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//it uses the modern profile of glfw
	
	if (!screen.init()) {
		cout << "Could not create window" << endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to Initialize GLAD" << endl;
		glfwTerminate();
		return -1;
	}

	// render the window
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//for dynamically resizing the window
	screen.setParameters();

	//Setting the z buffer or depth testing
	glEnable(GL_DEPTH_TEST);

	/*
	   shaders
	*/

	Shader shader("assets/object.vs", "assets/object.fs");
	Shader lampShader("assets/object.vs", "assets/lamp.fs");

	Gun g;
	//Model m(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f), true);
	g.loadModel("assets/models/m4a1/scene.gltf");
	
	
	DirLight dirLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec4(0.1f,0.1f, 0.1f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), 
		glm::vec4(0.75f, 0.75f, 0.75f, 1.0f) };


	glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
	};
	Lamp lamps[4];
	for (unsigned int i = 0; i < 4; i++) {
		lamps[i] = Lamp(glm::vec3(1.0f),
			glm::vec4(0.05f, 0.05f, 0.05f, 1.0f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			1.0f, 0.07f, 0.032f,
			pointLightPositions[i], glm::vec3(0.25f));
		lamps[i].init();
	}

	/*Cube cube(Material::mix(Material::gold, Material::bronze), glm::vec3(0.0f, 0.0f, -1.f), glm::vec3(0.75f));
	cube.init();*/



	/*Lamp lamp(glm::vec3(1.0f), 
		glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 
		1.0f, 0.07f, 0.032f,
		glm::vec3(-1.0f, -0.5f, -0.5f), glm::vec3(0.25f));
	lamp.init();*/

	SpotLight s = {
	camera[active].cameraPos, camera[active].cameraFront,
	1.0f, 0.07f, 0.032f,
	glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)),
	glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	};
	

	while (!screen.shouldClose()) {

		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		//process input window
		processInput(deltaTime);

		//render
		screen.update();

		shader.activate();

		//shader.set3Float("light.position", lamp.pos);
		shader.set3Float("viewPos", camera[active].cameraPos);

		dirLight.direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(1.0f, 0.0, 0.0)) *
		glm::vec4(dirLight.direction, 1.0f));

		dirLight.render(shader);

		for (int i = 0; i < 4; i++) {
			lamps[i].pointLight.render(shader, i);
		}

		shader.setInt("noPointLights", 4);

		if (flashLight) {
			s.position = camera[active].cameraPos;
			s.direction = camera[active].cameraFront;
			s.render(shader, 0);

			shader.setInt("noSpotLights", 1);
		}
		else {
			shader.setInt("noSpotLights", 0);
		}

		


		glm::mat4 view = glm::mat4(1.0f);//creating that camera view effect
		glm::mat4 projection = glm::mat4(1.0f);//for creating the perspective of a user of what can they see


		//view = glm::translate(view, glm::vec3(-x, -y, -z));
		view = camera[active].getViewMatrix();
		projection = glm::perspective(glm::radians(camera[active].getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


		

		//draw the first triangle
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		
		g.render(shader);
		
		lampShader.activate();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		
		for (int i = 0; i < 4; i++) {
			lamps[i].render(lampShader);
		}
		//sent new frames to window
		screen.newFrame();

		//tells if any events triggers or not like keyword press, mouse click
		glfwPollEvents();
		
	}

	
	//glDeleteBuffers(1, &EBO);
	g.cleanup();
	for (int i = 0; i < 4; i++) {
		lamps[i].cleanup();
	}
	glfwTerminate();
	return 0;

}

void processInput(double dt) {
	if (Keyboard::key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		screen.setShouldClose(true);
	}

	if (Keyboard::keyWentDown(GLFW_KEY_TAB)) {
		active = (active == 0) ? 1 : 0;
	}

	if (Keyboard::key(GLFW_KEY_W)) {
		camera[active].updateCameraPos(CameraDirection::FORWARD, dt);
	}

	if (Keyboard::key(GLFW_KEY_S)) {
		camera[active].updateCameraPos(CameraDirection::BACKWARD, dt);
	}
	if (Keyboard::key(GLFW_KEY_D)) {
		camera[active].updateCameraPos(CameraDirection::RIGHT, dt);
	}
	if (Keyboard::key(GLFW_KEY_A)) {
		camera[active].updateCameraPos(CameraDirection::LEFT, dt); 
	}
	if (Keyboard::key(GLFW_KEY_SPACE)) {
		camera[active].updateCameraPos(CameraDirection::UP, dt);
	}
	if (Keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
		camera[active].updateCameraPos(CameraDirection::DOWN, dt);
	}

	double dx = Mouse::getDX(), dy = Mouse::getDY();
	if (dx != 0 || dy != 0) {
		camera[active].updateCameraDirection(dx, dy);
	}

	double scrollDy = Mouse::getScrollDY();
	if (scrollDy != 0) {
		camera[active].updateCameraZoom(scrollDy);
	}

	//toggle flashLight

	if (Keyboard::keyWentDown(GLFW_KEY_RIGHT_ALT)) {
		flashLight = !flashLight;
	}
}
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//	glViewport(0, 0, width, height);
//	SCR_WIDTH = width;
//	SCR_HEIGHT = height;
//}

//string loadShaderSrc(const char* filename) {
//	ifstream file;
//	stringstream buf;// to read the data of the file
//	string ret = "";//placeholder for the return variable
//	
//	file.open(filename);
//
//	if (file.is_open()) {
//		buf << file.rdbuf();//read buffer/ piping to the string buffer
//		ret = buf.str();
//		
//	}
//	else {
//		cout << "Could not open " << filename << endl;
//	}
//	file.close();
//	return ret;
//} 