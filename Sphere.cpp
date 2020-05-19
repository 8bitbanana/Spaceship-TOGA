#include "Sphere.h"

#include <glm/gtx/quaternion.hpp>
#include "ResourceManager.h"
#include <iostream>
#include <cmath>
#include <vector>

//const GLfloat vertices[] = {
//	0.0f, 0.0f, 0.5f,
//	-0.5f, -0.5f, 0.0f,
//	0.5f, -0.5f, 0.0f,
//	0.5f, 0.5f, 0.0f,
//	-0.5f, 0.5f, 0.0f,
//	0.0f, 0.0f, -0.5f
//};
//
//const GLuint indexes[] = {
//	0, 1, 2,
//	0, 2, 3,
//	0, 3, 4,
//	0, 4, 1,
//	1, 5, 2,
//	2, 5, 3,
//	3, 5, 4,
//	4, 5, 1
//};

const glm::vec3 vertices[] = {
	glm::vec3(0.0f, 0.0f, 0.5f),
	glm::vec3(-0.5f,-0.5f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.5f),
	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(0.5f, 0.5f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.5f),
	glm::vec3(0.5f, 0.5f, 0.0f),
	glm::vec3(-0.5f, 0.5f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.5f),
	glm::vec3(-0.5f, 0.5f, 0.0f),
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(0.0f, 0.0f, -0.5f),
	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(0.0f, 0.0f, -0.5f),
	glm::vec3(0.5f, 0.5f, 0.0f),
	glm::vec3(0.5f, 0.5f, 0.0f),
	glm::vec3(0.0f, 0.0f, -0.5f),
	glm::vec3(-0.5f, 0.5f, 0.0f),
	glm::vec3(-0.5f, 0.5f, 0.0f),
	glm::vec3(0.0f, 0.0f, -0.5f),
	glm::vec3(-0.5f, -0.5f, 0.0f),
};

//const glm::vec3 vertices[] = {
//	glm::vec3(0.0f, 0.0f, 0.0f),
//	glm::vec3(0.5f, 0.0f, 0.0f),
//	glm::vec3(0.0f, 0.5f, 0.0f)
//};

void Subdivide(std::vector<glm::vec3> &vertarray, glm::vec3 a, glm::vec3 b, glm::vec3 c, int level) {
	if (level == 0) {
		vertarray.push_back(a);
		vertarray.push_back(b);
		vertarray.push_back(c);
	}
	else {
		glm::vec3 ab = 0.5f * (a + b);
		glm::vec3 ac = 0.5f * (a + c);
		glm::vec3 bc = 0.5f * (b + c);

		// Well isn't this lovely
		Subdivide(vertarray, a, ab, ac, level - 1);
		Subdivide(vertarray, ab, b, bc, level - 1);
		Subdivide(vertarray, ac, bc, c, level - 1);
		Subdivide(vertarray, ab, bc, ac, level -1);
	}
}

void Normalise(glm::vec3 *point_ptr, glm::vec3 center, float length) {
	auto point = *point_ptr;
	float dx = point.x - center.x;
	float dy = point.y - center.y;
	float dz = point.z - center.z;

	dx *= length / glm::distance(center, point);
	dy *= length / glm::distance(center, point);
	dz *= length / glm::distance(center, point);
	*point_ptr = glm::vec3(center.x + dx, center.y + dy, center.z + dz);
}

std::vector<glm::vec3> GenerateSubdivisions(glm::vec3* base, int basesize, GLuint subdivisions) {

	std::vector<glm::vec3> rawVertList;

	for (int i = 0; i < basesize; i += 3) {
		Subdivide(rawVertList, base[i], base[i+1], base[i+2], subdivisions);
	}
	for (int i = 0; i < rawVertList.size(); i++) {
		Normalise(&rawVertList[i], glm::vec3(0), 1);
	}
	return rawVertList;
}

std::vector<glm::vec3> subdividedVerts = GenerateSubdivisions((glm::vec3*)vertices, 24, 4);

Sphere::Sphere()
{
	
}

Sphere::Sphere(glm::vec3 pos, glm::vec3 rotation, glm::vec3 size, std::string shader) :
	Position(pos), Rotation(rotation), Size(size), ShaderName(shader), Subdivisions(1)
{
	Rotation = glm::radians(Rotation);

	Init();
}

void Sphere::Init()
{
	/*for (int i = 0; i < subdividedVerts.size(); i++) {
		glm::vec3 vert = *((&subdividedVerts[0]) + i);
		printf("x%f y%f z%f\n", vert.x, vert.y, vert.z);
	}
	printf("\n");*/
	// std::cout << subdividedVerts.size() << std::endl;

	shader = ResourceManager::GetShader(ShaderName);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * subdividedVerts.size(), &subdividedVerts[0], GL_STATIC_DRAW);

	//Octagon::UpdateFace(faceToDisplay);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
}

void Sphere::Update(GLfloat dt)
{
	auto rot = dt / 3;
	//rot = 0;
	Rotation += glm::vec3(rot, 0, 0);
}

void Sphere::Draw(glm::mat4 projection, glm::mat4 view)
{
	shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	
	model = glm::scale(model, Size);
	model = glm::translate(model, Position);
	glm::quat rot = glm::quat(Rotation);
	model *= glm::toMat4(rot);

	auto pvm = projection * view * model;

	shader.SetMatrix4("pvm", pvm);
	shader.SetFloat("linewidth", 0.01f);
	shader.SetVector4f("color", glm::vec4(1));
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, subdividedVerts.size());
}
