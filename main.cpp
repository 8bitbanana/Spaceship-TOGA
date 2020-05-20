#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <chrono>
#include <thread>
#include <iostream>

#include "Game.h"
#include "Util.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void focus_callback(GLFWwindow* window, int focused);
void set_cursor_state(GLFWwindow* window, bool locked);

const GLuint SCREEN_WIDTH = 1280;
const GLuint SCREEN_HEIGHT = 720;
const float ASPECT = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

bool CursorLocked;

Game ArcadeGame(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[]) {
	Util::init_random();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	set_cursor_state(window, true);

	glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowFocusCallback(window, focus_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(message_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	ArcadeGame.Init();

	while (!glfwWindowShouldClose(window)) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		mouse_callback(window, xpos, ypos);

		ArcadeGame.Update(deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ArcadeGame.Draw();

		float sleep = (2.f/10.f) - deltaTime;
		/*std::cout << sleep << std::endl;*/
		/*while (glfwGetTime() - lastFrame < 1.0f / 60) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}*/

		while (glfwGetTime() < lastFrame + 1.0/60) {
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}

		glfwSwapBuffers(window);
	}
}

void set_cursor_state(GLFWwindow* window, bool locked) {
	if (locked) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		CursorLocked = true;
	} else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		CursorLocked = false;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		//glfwSetWindowShouldClose(window, GL_TRUE);
		set_cursor_state(window, false);
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			ArcadeGame.Keys[key] = GL_TRUE;
		} else if (action == GLFW_RELEASE) {
			ArcadeGame.Keys[key] = GL_FALSE;
		}
	}
}

bool firstMouse = true;
float lastX = (float)SCREEN_WIDTH / 2;
float lastY = (float)SCREEN_HEIGHT / 2;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	const float sensitivity = 0.1f;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	glm::vec2 mouseOffset(xpos - lastX, lastY - ypos);
	lastX = xpos;
	lastY = ypos;
	mouseOffset *= sensitivity;

	if (CursorLocked) {
		ArcadeGame.Mouse = mouseOffset;
	} else {
		ArcadeGame.Mouse = glm::vec2(0);
	}	
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	bool windowfocused = glfwGetWindowAttrib(window, GLFW_FOCUSED) == GLFW_TRUE;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && windowfocused) {
		set_cursor_state(window, true);
	}
}

void focus_callback(GLFWwindow* window, int focused) {
	if (focused == GLFW_TRUE) {
		set_cursor_state(window, true);
	} else {
		set_cursor_state(window, false);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	int w = height * ASPECT;
	int left = (width - w) / 2;
	glViewport(left, 0, w, height);
	// current_screen_size.x = w;
	// current_screen_size.y = height;
}

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}