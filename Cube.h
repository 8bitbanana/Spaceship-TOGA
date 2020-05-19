#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"

#include <string>

class Cube
{
public:
	glm::vec3 Position, Rotation, Size; 

	std::string ShaderName, TextureName;

	Cube();
	Cube(glm::vec3 pos, glm::vec3 rotation, glm::vec3 size, std::string shader, std::string texture);

	virtual void Draw(glm::mat4 projection, glm::mat4 view);
private:
	Shader shader;
	Texture2D FaceTex;
	unsigned int VBO, VAO;

	void Init();
};

