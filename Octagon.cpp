#include "Octagon.h"

#include <glm/gtx/quaternion.hpp>
#include "ResourceManager.h"
#include <iostream>

const GLfloat vertices[] = {
	0.0f, 0.0f, 0.5f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.0f, 0.0f, -0.5f
};

const GLuint indexes[] = {
	0, 1, 2,
	0, 2, 3,
	0, 3, 4,
	0, 4, 1,
	1, 5, 2,
	2, 5, 3,
	3, 5, 4,
	4, 5, 1
};

Octagon::Octagon()
{
}

Octagon::Octagon(glm::vec3 pos, glm::vec3 rotation, glm::vec3 size, std::string shader) :
	Position(pos), Rotation(rotation), Size(size), ShaderName(shader), faceToDisplay(0), count(0)
{
	Rotation = glm::radians(Rotation);

	Init();
}

void Octagon::Init()
{
	shader = ResourceManager::GetShader(ShaderName);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_DYNAMIC_DRAW);
	//Octagon::UpdateFace(faceToDisplay);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Octagon::Update(GLfloat dt)
{
	auto rot = dt / 3;
	//rot = 0;
	Rotation += glm::vec3(0, rot, 0);
}

void Octagon::Draw(glm::mat4 projection, glm::mat4 view)
{
	shader.Use();
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::scale(model, Size);
	model = glm::translate(model, Position);
	glm::quat rot = glm::quat(Rotation);
	model *= glm::toMat4(rot);

	shader.SetMatrix4("pvm", projection * view * model);
	shader.SetFloat("linewidth", 0.03f);
	shader.SetVector4f("color", glm::vec4(1,1,1,1));
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
}
