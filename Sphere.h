#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"

#include <string>


class Sphere
{
public:
	glm::vec3 Position, Rotation, Size;

	std::string ShaderName;

	GLuint Subdivisions;

	Sphere();
	Sphere(glm::vec3 pos, glm::vec3 rotation, glm::vec3 size, std::string shader);
	
	virtual void Draw(glm::mat4 projection, glm::mat4 view);

	virtual void Update(GLfloat dt);
private:
	Shader shader;
	unsigned int VBO, VAO, EBO;

	void Init();
};

