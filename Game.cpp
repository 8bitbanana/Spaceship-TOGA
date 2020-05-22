#include "Game.h"
#include "ResourceManager.h"
#include "Model.h"
#include "Ship.h"
#include "World.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

const glm::vec3 cameraOffset = {0.0, 1.0, 1.0};

Ship* ship;
vector<Model*> rooks;
World* world;

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
	ResourceManager::LoadMesh("Models/rook.obj", "rook");
	ResourceManager::LoadMesh("Models/ship.obj", "ship");

	CurrentProjection = glm::perspective(glm::radians(60.0f), float(Width) / Height, 0.1f, 100.0f);
	ship = new Ship();
	ship->Colour = glm::vec4(0.188, 0.933, 1.0, 1.0);
	ship->SetShader("wireframe-pulse");

	for (int i=0; i<10; i++) {
		rooks.push_back(new Model("Models/rook.obj"));
		rooks[i]->Position = {5.0 * (i%5), 0, 5.0 * (i/5)};
	}

	world = new World();
}

void Game::Update(GLfloat dt)
{
	ProcessInput(dt);
	ship->Update(dt);
	UpdateCamera();
	world->Update(dt, ship->Position);

	bool collision = false;
	for (int i=0; i<ship->CollisionPoint_Count; i++) {
		auto point = ship->GetCollisionPoint(i);
		point += ship->Position;

		if (world->IsCollision(point)) {
			collision = true;
			break;
		}
	}
	if (collision) {
		
	}
}

void Game::ProcessInput(GLfloat dt)
{
	ship->Input.Forward = Keys[GLFW_KEY_W];
    ship->Input.Left = Keys[GLFW_KEY_A];
	ship->Input.Backward = Keys[GLFW_KEY_S];
    ship->Input.Right = Keys[GLFW_KEY_D];
}

void Game::UpdateCamera() {
	const glm::vec3 cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
	const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);

	const vec3 lookOffset = {0.0, 1.0, -1.0};
	const vec3 cameraOffset = {0.0, 2.8, 8.0};

	CameraPos = ship->Position + cameraOffset;

	CurrentView = glm::lookAt(ship->Position+cameraOffset, ship->Position+lookOffset, cameraUp);
}

void Game::Draw()
{
	ship->Draw(CurrentProjection, CurrentView);
	for (auto rook : rooks)
		rook->Draw(CurrentProjection, CurrentView);
	world->Draw(CurrentProjection, CurrentView);
}

void Game::ResizeEvent(GLfloat width, GLfloat height)
{
	Width = width;
	Height = height;
}