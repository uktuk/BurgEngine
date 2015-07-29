#include "Common.h"
#include "Mesh.h"

Mesh::Mesh()
: mVertices(nullptr)
, mIndices(nullptr)
, mTextures(nullptr)
, mNumVertices(0)
, mNumIndices(0)
, mNumTextures(0)
{
}

void Mesh::Destroy()
{
	SafeDeleteArray(mVertices);
	SafeDeleteArray(mIndices);
	SafeDeleteArray(mTextures);

	mNumVertices = 0;
	mNumIndices = 0;
	mNumTextures = 0;
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	glBufferData(GL_ARRAY_BUFFER, mNumVertices * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumIndices * sizeof(GLuint), &mIndices[0], GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));  // offsetof macro returns the byte offset of that variable from the start of the struct

	// Vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}



void MeshBuilder::GenerateMesh(Mesh& mesh, Vertex* vertexData, uint numVertices, uint* indexData, uint numIndices, Texture* textureData, uint numTextures)
{
	// Free any existing data
	mesh.Destroy();

	// Assign Data
	mesh.mVertices = vertexData;
	mesh.mIndices = indexData;
	mesh.mTextures = textureData;

	mesh.mNumVertices = numVertices;
	mesh.mNumIndices = numIndices;
	mesh.mNumTextures = numTextures;

	mesh.SetupMesh();
}