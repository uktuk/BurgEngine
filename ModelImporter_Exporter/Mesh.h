#ifndef __MESH_H
#define __MESH_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>

#include <string>
#include <vector>
#include "Shader.h"


struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	GLuint id;
	std::string type;	
	aiString path;
};

class Mesh
{
public:
	// Functions
	Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures);
	void Draw(Shader &shader);

	// Mesh Data
	std::vector<Vertex> mVertices;
	std::vector<GLuint> mIndices;
	std::vector<Texture> mTextures;

private:
	void SetupMesh();

	// Render data
	GLuint mVAO, mVBO, mEBO;
};

#endif // #define __MESH_H