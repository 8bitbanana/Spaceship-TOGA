#include "Ship.h"

#include <algorithm>

Ship::Ship() : Model("Models\\ship.obj") {
    SetShader("wireframe-pulse");
}

void Ship::Update(GLfloat dt) {
    const float speed = 2.5;
    const float maxbank = glm::radians(17.0);
    const float bankspeed = glm::radians(100.0);

    const glm::vec3 FORWARD = {0, 0, -1.0};
    const glm::vec3 RIGHT = {1.0, 0, 0};
    const glm::vec3 UP = {0, 1.0, 0};

    float targetBank = 0;

    if (Input.Forward) {
        Position += FORWARD * speed * dt;
    }
    if (Input.Left) {
        Position -= RIGHT * speed * dt;
        targetBank = maxbank;
    }
    if (Input.Right) {
        Position += RIGHT * speed * dt;
        targetBank = -maxbank;
    }

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