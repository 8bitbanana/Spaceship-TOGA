#include "Game.h"
#include "ResourceManager.h"
#include "Cube.h"
#include "Octagon.h"
#include "Sphere.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#include "Sphere.h"

const glm::vec3 cameraOffset = {0.0, 1.0, 1.0};

Sphere* sphere;
Sphere* sphere2;

Model* ship;

Game::Game(GLuint width, GLuint height) : Width(width), Height(height), CameraPos(0, 0, 5.0f), CameraRot(0, -90.0f, 0)
{

}

Game::~Game()
{
	
}

void Game::Init()
{
	ResourceManager::LoadShader("Shaders/baseproj.vert", "Shaders/baseproj.frag", nullptr, "baseproj");
	ResourceManager::LoadShader("Shaders/wireframe.vert", "Shaders/wireframe.frag", "Shaders/wireframe.geom", "wireframe");
	ResourceManager::LoadShader("Shaders/wireframe-pulse.vert", "Shaders/wireframe-pulse.frag", "Shaders/wireframe-pulse.geom", "wireframe-pulse");


	CurrentProjection = glm::perspective(glm::radians(45.0f), float(Width / Height), 0.1f, 100.0f);

	sphere = new Sphere(glm::vec3(0, 0, 0), glm::vec3(90, 0, 0), glm::vec3(1, 1, 1), "wireframe");
	sphere2 = new Sphere(glm::vec3(0, 0, 0), glm::vec3(90, 0, 0), glm::vec3(0.5, 0.5, 0.5), "wireframe");

	ship = new Model("Models/ship.obj", glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(1,1,1));
	ship->Colour = glm::vec4(0.188, 0.933, 1.0, 1.0);
	ship->SetShader("wireframe-pulse");
}

void Game::Update(GLfloat dt)
{
	ProcessInput(dt);
	sphere->Update(dt);
	sphere2->Update(dt);
	ship->Update(dt);
}

void Game::ProcessInput(GLfloat dt)
{
	const float speed = 2.5f;
	const glm::vec3 cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
	const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);

	// Apply mouse movement to rotation
	CameraRot.y += Mouse.x; // yaw
	CameraRot.x += Mouse.y; // pitch

	if (CameraRot.x > 89.0f)
		CameraRot.x = 89.0f;
	if (CameraRot.x < -89.0f)
		CameraRot.x = -89.0f;

	glm::vec3 lookDirection;
	lookDirection.x = cos(glm::radians(CameraRot.y)) * cos(glm::radians(CameraRot.x));
	lookDirection.y = sin(glm::radians(CameraRot.x));
	lookDirection.z = sin(glm::radians(CameraRot.y)) * cos(glm::radians(CameraRot.x));
	lookDirection = glm::normalize(lookDirection);

	glm::vec3 moveDirection = lookDirection;
	moveDirection.y = 0;

	// Apply keyboard to current movement
	if (Keys[GLFW_KEY_W]) {
		CameraPos += moveDirection * speed * dt;
	}
	if (Keys[GLFW_KEY_A]) {
		CameraPos -= glm::normalize(glm::cross(moveDirection, cameraUp)) * speed * dt;
	}
	if (Keys[GLFW_KEY_S]) {
		CameraPos -= moveDirection * speed * dt;
	}
	if (Keys[GLFW_KEY_D]) {
		CameraPos += glm::normalize(glm::cross(moveDirection, cameraUp)) * speed * dt;
	}
	if (Keys[GLFW_KEY_SPACE]) {
		CameraPos += cameraUp * speed * dt;
	}
	if (Keys[GLFW_KEY_LEFT_SHIFT]) {
		CameraPos -= cameraUp * speed * dt;
	}
	// printf("----\n");
	// printf("POS x%f y%f z&f \n", CameraPos.x, CameraPos.y, (float)CameraPos.z);
	// printf("ROT x%f y%f z%f \n", lookDirection.x, lookDirection.y, lookDirection.z);

	CurrentView = glm::lookAt(CameraPos, CameraPos + lookDirection, cameraUp);
}

void Game::Draw()
{
	//sphere->Draw(CurrentProjection, CurrentView);
	//sphere2->Draw(CurrentProjection, CurrentView);
	ship->Draw(CurrentProjection, CurrentView);
}

void Game::ResizeEvent(GLfloat width, GLfloat height)
{
	Width = width;
	Height = height;
}