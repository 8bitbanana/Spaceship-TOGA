#include "Game.h"
#include "ResourceManager.h"
#include "Ship.h"
#include "World.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

const glm::vec3 cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);

Ship* ship;
World* world;

Game::Game(GLuint width, GLuint height)
	: Mode(Start), Width(width), Height(height),
	CameraPos(0, 0, 5.0f), CameraRot(0, -90.0f, 0)
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

	InitMode(Mode);
}

void Game::InitMode(GameMode mode) {
	switch (mode) {
		case Start: {
			ship = new Ship();
			world = new World();
			StartCutsceneRotation = 300;
		}
		break;
		case Intro: {
			IntroCutsceneProgress = 3.0;
			IntroCutsceneCountdown = 3;
		}
		break;
		case Play: {
			ship->StartMoving();	
		}
		break;
		case Dead: {
			ship->SetShader("wireframe");
			DeathCutsceneProgress = 3.0;
		}
		break;
	}
	Mode = mode;
}

void Game::Update(GLfloat dt)
{
	switch (Mode) {
		case Start: {
			StartCutscene(dt);
			for (int i=0; i<1024; i++) {
				if (!Keys[i]) {continue;}
				if (i == GLFW_KEY_ESCAPE) {continue;}
				if (i >= GLFW_KEY_PAGE_UP) {continue;}
				InitMode(Intro);
			}
			world->Update(dt, ship->Position);
		}
		break;
		case Intro: {
			bool done = IntroCutscene(dt);
			UpdateCamera();
			world->Update(dt, ship->Position);
			ship->Update(dt);
			if (done) {
				InitMode(Play);
			}
		}
		break;
		case Play: {
			const float ShipDamage_Multi = 250.0;

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
				bool dead = ship->TakeDamage(ShipDamage_Multi * dt);
				if (dead) {
					InitMode(Dead);
				}
			}	
		}
		break;
		case Dead: {
			bool done = DeathCutscene(dt);
			if (done) {
				InitMode(Start);
			}
		}
		break;
	}
}

void Game::ProcessInput(GLfloat dt)
{
	ship->Input.Forward = Keys[GLFW_KEY_W] | Keys[GLFW_KEY_UP];
    ship->Input.Left = Keys[GLFW_KEY_A] | Keys[GLFW_KEY_LEFT];
	ship->Input.Backward = Keys[GLFW_KEY_S] | Keys[GLFW_KEY_DOWN];
    ship->Input.Right = Keys[GLFW_KEY_D] | Keys[GLFW_KEY_RIGHT];
}

bool Game::IntroCutscene(GLfloat dt) {
	IntroCutsceneProgress -= dt;
	if (IntroCutsceneProgress < IntroCutsceneCountdown) {
		IntroCutsceneCountdown -= 1;
		ship->Pulse();
	}

	if (IntroCutsceneProgress <= 0) {
		IntroCutsceneProgress = 0;
		return true;
	} else {
		return false;
	}
}

void Game::UpdateCamera() {
	const vec3 lookOffset = {0.0, 1.0, -1.0};
	const vec3 cameraOffset = {0.0, 2.8, 8.0};

	CameraPos = ship->Position + cameraOffset;

	CurrentView = glm::lookAt(ship->Position+cameraOffset, ship->Position+lookOffset, cameraUp);
}

void Game::StartCutscene(GLfloat dt) {
	const GLfloat rotationSpeed = 10.0f;
	const vec3 view = {0.0, 2.0, 5.0};
	const vec3 look = {0.0, 1.0, 0.5};

	glm::mat4 rotationMat(1);
	rotationMat = glm::rotate(rotationMat, glm::radians(StartCutsceneRotation), {0.0, 1.0, 0.0});
	vec3 rotated_view = glm::vec4(view, 1) * rotationMat;

	CurrentView = glm::lookAt(ship->Position+rotated_view, ship->Position+look, cameraUp);

	StartCutsceneRotation += rotationSpeed * dt;
	if (StartCutsceneRotation > 360)
		StartCutsceneRotation -= 360;
}

bool Game::DeathCutscene(GLfloat dt) {
	const vec3 viewStart = {0.0, 2.8, 8.0};
	const vec3 viewEnd = {2.0, 3.8, 7.0};
	const vec3 lookStart = {0.0, 1.0, -1.0};
	const vec3 lookEnd = {0.0, 0.5, -1.0};

	vec3 view = glm::mix(viewStart, viewEnd, 1-DeathCutsceneProgress);
	vec3 look = glm::mix(lookStart, lookEnd, 1-DeathCutsceneProgress);
	CurrentView = glm::lookAt(ship->Position+view, ship->Position+look, cameraUp);

	DeathCutsceneProgress -= dt;
	if (DeathCutsceneProgress <= 0) {
		DeathCutsceneProgress = 0;
		return true;
	} else {
		return false;
	}
}

void Game::Draw()
{
	if (ship != nullptr)
		ship->Draw(CurrentProjection, CurrentView);
	if (world != nullptr)
		world->Draw(CurrentProjection, CurrentView);
}

void Game::ResizeEvent(GLfloat width, GLfloat height)
{
	Width = width;
	Height = height;
}