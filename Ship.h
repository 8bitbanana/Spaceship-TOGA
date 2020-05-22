#pragma once
#include "Model.h"

class Ship: public Model {
public:
    Ship();
    void Update(GLfloat dt);
    void SetInput(GLboolean keys[1024]);

    struct InputStruct {
        bool Forward, Backward, Left, Right = false;
    };

    const unsigned int CollisionPoint_Count = 3;
    glm::vec3 GetCollisionPoint(unsigned int i);
    InputStruct Input;
private:
    float ForcedSpeed;
    glm::vec3 collisionPoints[3];
};