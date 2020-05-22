#include "Ship.h"

#include <algorithm>
#include <glm/gtx/quaternion.hpp>

Ship::Ship() : Model("ship") {
    SetShader("wireframe-pulse");

    ForcedSpeed = 15.0;

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
}

void Ship::Update(GLfloat dt) {
    const float strafespeed = 10.0;
    const float manualSpeed = 2.5;
    const float maxForcedSpeed = 25.0f;
    const float forcedAccelleration = 0.5f;
    
    const float maxbank = glm::radians(17.0);
    const float bankspeed = glm::radians(100.0);

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
}

glm::vec3 Ship::GetCollisionPoint(unsigned int i) {
    assert(i < CollisionPoint_Count);
    return glm::vec4(collisionPoints[i],1) * currentModel;
}