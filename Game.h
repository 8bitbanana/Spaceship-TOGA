#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>

enum GameMode {
	Start, Intro, Play, Dead
};

class Game
{
public:
	GameMode Mode;
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
	glm::highp_mat4 CurrentProjection;
	glm::highp_mat4 CurrentView;
	void ProcessInput(GLfloat dt);
	void UpdateCamera();

	GLfloat StartCutsceneRotation;
	GLfloat IntroCutsceneProgress;
	GLuint IntroCutsceneCountdown;
	GLfloat DeathCutsceneProgress;

	bool DeathCutscene(GLfloat dt);
	bool IntroCutscene(GLfloat dt);
	void InitMode(GameMode mode);
	void StartCutscene(GLfloat dt);
};