#include "Ship.h"

#include <algorithm>
#include <glm/gtx/quaternion.hpp>

Ship::Ship() : Model("ship") {
    Position = glm::vec3(0, 0.5, 0);
    SetShader("wireframe-pulse");
    Colour = glm::vec4(0, 1, 1, 1);

    ForcedSpeed = 15.0;
    Health = MaxHealth;

    auto leftmost = glm::vec3(0);
    auto rightmost = glm::vec3(0);
    for (auto vert : mesh.Vertices) {
        if (vert.x < leftmost.x)
            leftmost = vert;
        if (vert.x > rightmost.x)
            rightmost = vert;
    }
    collisionPoints[0] = glm::vec3(0);
    collisionPoints[1] = leftmost;
    collisionPoints[2] = rightmost;
    collisionPoints[3] = leftmost / 2.0f;
    collisionPoints[4] = rightmost / 2.0f;
}

void Ship::Update(GLfloat dt) {
    const float strafespeed = 15.0;
    const float manualSpeed = 2.5;
    const float maxForcedSpeed = 40.0f;
    const float forcedAccelleration = 0.5f;
    
    const float maxbank = glm::radians(25.0);
    const float bankspeed = glm::radians(150.0);

    const glm::vec3 FORWARD = {0, 0, -1.0};
    const glm::vec3 RIGHT = {1.0, 0, 0};
    const glm::vec3 UP = {0, 1.0, 0};

    float targetBank = 0;

    if (Input.Forward) {
        Position += FORWARD * manualSpeed * dt;
    }
    if (Input.Left) {
        Position -= RIGHT * strafespeed * dt;
        targetBank = maxbank;
    }
    if (Input.Right) {
        Position += RIGHT * strafespeed * dt;
        targetBank = -maxbank;
    }
    if (Input.Backward) {
        Position -= FORWARD * manualSpeed * dt;
    }
    if (Input.Left && Input.Right) {targetBank = 0;}
    Position += FORWARD * ForcedSpeed * dt;

    if (targetBank > Rotation.z) {
        Rotation.z += bankspeed * dt;
        if (Rotation.z > targetBank)
            Rotation.z = targetBank;
    } else if (targetBank < Rotation.z) {
        Rotation.z -= bankspeed * dt;
        if (Rotation.z < targetBank)
            Rotation.z = targetBank;
    }

    if (ForcedSpeed < maxForcedSpeed) {
        ForcedSpeed += dt * forcedAccelleration;
    }

    float HealthRatio = Health / MaxHealth;
    Colour = glm::mix(
        glm::vec4(1, 0.65, 0, 1), // no health (orange)
        glm::vec4(0, 1, 1, 1), // full health (blue)
        HealthRatio
    );


    if (Health / MaxHealth < 0.3) {
        float flashSpeed = (HealthRatio < 0.15) ? 25 : 10;
        // This could be faster
        int mod = (int)(glfwGetTime() * flashSpeed) % 6;
        Colour.w = (mod == 0) ? 0 : 1;
    }
}

bool Ship::TakeDamage(GLfloat damage) {
    Health -= damage;
    if (Health <= 0) {
        Health = 0;
        Colour.w = 1;
        return true;
    } else {
        return false;
    }
}

glm::vec3 Ship::GetCollisionPoint(unsigned int i) {
    assert(i < CollisionPoint_Count);
    return glm::vec4(collisionPoints[i],1) * currentModel;
}