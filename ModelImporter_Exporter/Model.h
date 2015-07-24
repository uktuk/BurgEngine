#ifndef __MODEL_H
#define __MODEL_H

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// SOIL
#include <SOIL/SOIL.h>
// Other
#include "Mesh.h"

class Model
{
public:
	Model(GLchar* path);
	void Draw(Shader& shader);
	void Translate(glm::vec3 transBy);
	void SetPosition(glm::vec3 transTo);
	void Rotate(float angle, glm::vec3 rot);

private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	std::vector<Mesh> mMeshes;
	std::vector<Texture> mTexturesLoaded;
	std::string mDirectory;
	glm::vec3 mPosition;
	glm::vec3 mRotation;
	float mRotAngle;
};

#endif // #define __MODEL_H