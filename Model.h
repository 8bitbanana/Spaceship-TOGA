#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "ResourceManager.h"

using std::string;
using std::vector;
using glm::vec3;



class Model {
public:
    Model();
    Model(const string& mesh);
    Model(const string& mesh, vec3 position, vec3 rotation, vec3 size);

    virtual void Draw(glm::mat4 projection, glm::mat4 view);
	virtual void Update(GLfloat dt);

    void SetShader(string name);

    glm::vec3 Position = glm::vec3(0);
    glm::vec3 Rotation = glm::vec3(0);
    glm::vec3 Size = glm::vec3(1);
    glm::vec4 Colour = glm::vec4(1);
    
private:
    unsigned int VBO, VAO, EBO;

    Mesh mesh;
    Shader shader;

    void Init();
};