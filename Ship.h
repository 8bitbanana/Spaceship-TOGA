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
    InputStruct Input;
};