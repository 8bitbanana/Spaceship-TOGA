#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <assimp/scene.h>

#include "Shader.h"
#include "ResourceManager.h"

using std::string;
using std::vector;
using glm::vec3;

struct Mesh {
    vector<glm::vec3> Vertices;
    vector<GLuint> Indices;
    glm::mat4x4 Transform;
};

class Model {
public:
    Model();
    Model(const string& filename);
    Model(const string& filename, vec3 position, vec3 rotation, vec3 size);

    virtual void Draw(glm::mat4 projection, glm::mat4 view);
	virtual void Update(GLfloat dt);

    void SetShader(string name);

    glm::vec3 Position = glm::vec3(0);
    glm::vec3 Rotation = glm::vec3(0);
    glm::vec3 Size = glm::vec3(1);
    glm::vec4 Colour = glm::vec4(1);
    
private:
    vector<Mesh> Meshes;
    vector<vec3> Vertices;
    vector<GLuint> Indices;
    
    unsigned int VBO, VAO, EBO;

    Shader shader;

    void Import(const string& filename);
    void Init();
    void LoadMeshesFromNode(const aiNode* node, aiMesh** meshes, glm::mat4 currentTransform);

    static glm::mat4 AiToGlm(aiMatrix4x4 aiM);
    static glm::vec3 AiToGlm(aiVector3D aiV);
};