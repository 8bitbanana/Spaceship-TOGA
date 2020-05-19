#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"

#include <string>


class Octagon
{
public:
	glm::vec3 Position, Rotation, Size;

	std::string ShaderName;

	Octagon();
	Octagon(glm::vec3 pos, glm::vec3 rotation, glm::vec3 size, std::string shader);
	
	virtual void Draw(glm::mat4 projection, glm::mat4 view);

	virtual void Update(GLfloat dt);
private:
	Shader shader;
	unsigned int VBO, VAO, EBO;

	void Init();
	void UpdateFace(GLuint index);
	void Subdivide(glm::vec3 a, glm::vec3 b, glm::vec3 c, GLuint level);
	void DrawTri(glm::vec3 a, glm::vec3 b, glm::vec3 c);

	GLfloat count;
	GLuint faceToDisplay;
};

