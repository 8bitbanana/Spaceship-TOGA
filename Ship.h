#pragma once
#include "Model.h"

class Ship: public Model {
public:
    Ship();
    void Update(GLfloat dt);
    bool TakeDamage(GLfloat damage);


    struct InputStruct {
        bool Forward, Backward, Left, Right = false;
    };

    const unsigned int CollisionPoint_Count = 5;
    glm::vec3 GetCollisionPoint(unsigned int i);

    InputStruct Input;
private:
    const float MaxHealth = 100;
    float Health;
    float ForcedSpeed;
    glm::vec3 collisionPoints[5];
};