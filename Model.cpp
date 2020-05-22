#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ResourceManager.h"

Model::Model() {

}

Model::Model(const string& meshname) {
    SetShader("wireframe");
    mesh = ResourceManager::GetMesh(meshname);
    Init();
}

Model::Model(const string& meshname, vec3 position, vec3 rotation, vec3 size)
 : Position(position), Rotation(rotation), Size(size) {
    Rotation = glm::radians(Rotation);
    SetShader("wireframe");
    mesh = ResourceManager::GetMesh(meshname);
    Init();
}

void Model::Init() {
    
}

void Model::SetShader(string name) {
    shader = ResourceManager::GetShader(name);
}

void Model::Update(GLfloat dt) {
	
}

void Model::Draw(glm::mat4 projection, glm::mat4 view) {
    shader.Use();

	currentModel = glm::mat4(1.0);
	currentModel = glm::scale(currentModel, Size);
	currentModel = glm::translate(currentModel, Position);
	glm::quat rot = glm::quat(Rotation);
	currentModel *= glm::toMat4(rot);

	auto pvm = projection * view * currentModel;

	shader.SetMatrix4("pvm", pvm);
	shader.SetFloat("linewidth", 0.02f);
	shader.SetVector4f("color", Colour);
    shader.SetFloat("time", glfwGetTime());
	
	mesh.Draw();
}