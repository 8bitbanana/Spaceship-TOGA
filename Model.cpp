#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>



glm::mat4 Model::AiToGlm(aiMatrix4x4 aiM) {
    float values[16] = {
        aiM.a1, aiM.a2, aiM.a3, aiM.a4,
        aiM.b1, aiM.b2, aiM.b3, aiM.b4,
        aiM.c1, aiM.c2, aiM.c3, aiM.c4,
        aiM.d1, aiM.d2, aiM.d3, aiM.d4
    };
    return glm::make_mat4(values);
}

glm::vec3 Model::AiToGlm(aiVector3D aiV) {
    return glm::vec3(aiV.x, aiV.y, aiV.z);
}

Model::Model() {

}

Model::Model(const string& filename) {
    SetShader("wireframe");
    Import(filename);
    Init();
}

Model::Model(const string& filename, vec3 position, vec3 rotation, vec3 size)
 : Position(position), Rotation(rotation), Size(size) {
    Rotation = glm::radians(Rotation);
    SetShader("wireframe");
    Import(filename);
    Init();
}

void Model::Init() {
    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
}

void Model::SetShader(string name) {
    shader = ResourceManager::GetShader(name);
}

void Model::Update(GLfloat dt) {

}

void Model::Draw(glm::mat4 projection, glm::mat4 view) {
    shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	
	model = glm::scale(model, Size);
	model = glm::translate(model, Position);
	glm::quat rot = glm::quat(Rotation);
	model *= glm::toMat4(rot);

	auto pvm = projection * view * model;

	shader.SetMatrix4("pvm", pvm);
	shader.SetFloat("linewidth", 0.02f);
	shader.SetVector4f("color", Colour);
    shader.SetFloat("time", glfwGetTime());
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
}

void Model::LoadMeshesFromNode(const aiNode* node, aiMesh** meshes, glm::mat4 currentTransform) {
    // Add this node's transform to the stack
    currentTransform = currentTransform * AiToGlm(node->mTransformation);
    // Iterate through each mesh in this node
    for (unsigned int meshIndex=0; meshIndex<node->mNumMeshes; meshIndex++) {
        aiMesh* mesh = meshes[node->mMeshes[meshIndex]];
        // Construct the Mesh struct
        Mesh meshStruct;
        meshStruct.Transform = currentTransform;
        // Add each vertex to the struct
        for (unsigned int vertIndex=0; vertIndex<mesh->mNumVertices; vertIndex++) {
            meshStruct.Vertices.push_back(AiToGlm(mesh->mVertices[vertIndex]));
        }
        // Add each index inside each face to the struct
        for (unsigned int faceIndex=0; faceIndex<mesh->mNumFaces; faceIndex++) {
            aiFace face = mesh->mFaces[faceIndex];
            // aiProcess_Triangulate makes sure that all faces are triangles
            assert(face.mNumIndices == 3);
            for (unsigned int i=0; i<face.mNumIndices; i++) {
                meshStruct.Indices.push_back(face.mIndices[i]);
            }
        }
        Meshes.push_back(meshStruct);
    }

    // Recursively run this function for all of this node's children
    for (unsigned int childIndex=0; childIndex<node->mNumChildren; childIndex++) {
        aiNode* child = node->mChildren[childIndex];
        LoadMeshesFromNode(child, meshes, currentTransform);
    }
}

void Model::Import(const string& filename) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename,
        // aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType
    );

    if (!scene) {
        fprintf(stderr, "ASSIMP ERROR - %s\n", importer.GetErrorString());
        return;
    }

    LoadMeshesFromNode(scene->mRootNode, scene->mMeshes, glm::mat4(1.0));
    

    unsigned int indexOffset = 0;
    for (auto mesh : Meshes) {
        for (auto vert : mesh.Vertices) {
            glm::vec4 vert4 = glm::vec4(vert, 1);
            Vertices.push_back((glm::vec3)(vert4 * mesh.Transform));
        }   
        for (auto ind : mesh.Indices) {
            Indices.push_back(ind + indexOffset);
        }
        indexOffset += mesh.Vertices.size();
    }
    Meshes = vector<Mesh>(); // clear mesh list, no longer used.
}

