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
#include "Common.h"

//====================================================================================================
// Structs
//====================================================================================================

struct BoneWeight
{
	u32 boneIndex;
	f32 weight; // how much is this affected by a particular bone
};
typedef std::vector<BoneWeight> BoneWeights;
typedef std::vector<BoneWeights> VertexWeights;


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
	Mesh();
	// Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures);
	void Draw(Shader &shader);
	void Destroy();
	void SetupMesh();

	const VertexWeights& GetVertexWeights() const  { return mVertexWeights; }
	VertexWeights& GetVertexWeights() { return mVertexWeights; }

	// Mesh Data
	Vertex* mVertices;
	GLuint* mIndices;
	Texture* mTextures;

	uint mNumVertices;
	uint mNumIndices;
	uint mNumTextures;

private:
	//NONCOPYABLE(Mesh);

	// Render data
	GLuint mVAO, mVBO, mEBO;
	VertexWeights mVertexWeights; // Needs to be the same as mNumVertices
};

namespace MeshBuilder
{
	void GenerateMesh(Mesh& mesh, Vertex* vertexData, uint numVertices, uint* indexData, uint numIndices, Texture* textureData, uint numTextures);
}

#endif // #define __MESH_H