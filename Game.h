#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>

class Game
{
public:
	GLboolean Keys[1024];
	GLuint Width, Height;
	glm::vec2 Mouse;

	Game(GLuint width, GLuint height);
	~Game();

	void Init();
	void Update(GLfloat dt);
	void Draw();
	void ResizeEvent(GLfloat width, GLfloat height);
private:
	glm::vec3 CameraPos;
	glm::vec3 CameraRot;
	glm::mat4 CurrentProjection;
	glm::mat4 CurrentView;
	void ProcessInput(GLfloat dt);
};