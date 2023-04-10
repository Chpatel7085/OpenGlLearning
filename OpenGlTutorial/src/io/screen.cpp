#include"screen.h"

#include"Keyboard.h"
#include"mouse.h"

unsigned int Screen::SCR_WIDTH = 800;
unsigned int Screen::SCR_HEIGHT = 600;

void Screen::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	SCR_WIDTH = width, SCR_HEIGHT = height;
}

Screen::Screen()
	:window(nullptr){}

bool Screen::init() {
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
	if (window == NULL) {
		return false;
	}
	glfwMakeContextCurrent(window);
	return true;
}

void Screen::setParameters() {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//for dynamically resizing the window
	glfwSetFramebufferSizeCallback(window, Screen::framebufferSizeCallback);

	// for the keyboards and mouse
	glfwSetKeyCallback(window, Keyboard::keyCallback);
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	//diable the cursor, makes it more realistic like a game
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	
}

void Screen::update() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Screen::newFrame() {
	glfwSwapBuffers(window);
}

void Screen::setShouldClose(bool shouldClose) {
	glfwSetWindowShouldClose(window, shouldClose);
}
bool Screen::shouldClose() {
	return glfwWindowShouldClose(window);
}
