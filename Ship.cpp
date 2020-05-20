#include "Ship.h"

#include <algorithm>

Ship::Ship() : Model("ship") {
    SetShader("wireframe-pulse");
}

void Ship::Update(GLfloat dt) {
    const float strafespeed = 10.0;
    const float manualSpeed = 2.5;
    const float forcedSpeed = 15.0;
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
    Position += FORWARD * forcedSpeed * dt;

    if (targetBank > Rotation.z) {
        Rotation.z += bankspeed * dt;
        if (Rotation.z > targetBank)
            Rotation.z = targetBank;
    } else if (targetBank < Rotation.z) {
        Rotation.z -= bankspeed * dt;
        if (Rotation.z < targetBank)
            Rotation.z = targetBank;
    }
}