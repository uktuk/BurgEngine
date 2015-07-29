#ifndef __MODEL_H
#define __MODEL_H

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// SOIL
#include <SOIL/SOIL.h>
// Other
#include <fstream>
#include <map>
#include "Mesh.h"
#include "Math.h"
#include "Common.h"

// Foward delcarations
struct Bone;

// Class declaration
class Model
{
public:
	Model(GLchar* path);
	~Model();
	void LoadModel(std::string path);
	void Unload();

	void Draw(Shader& shader);
	void Translate(glm::vec3 transBy);
	void SetPosition(glm::vec3 transTo);
	void Rotate(float angle, glm::vec3 rot);

public:
	void LoadMaterialTextures(std::vector<Texture*>& textureData, char* path, aiTextureType type, std::string typeName);

	std::vector<Mesh*> mMeshes;
	std::vector<GLuint> mIndices;
	std::vector<Texture*> mTexturesLoaded;
	std::string mDirectory;

	Bone* mRoot;
	std::vector<Bone*> mBones;
	std::map<std::string, u32> mBoneIndexMap;

	glm::vec3 mPosition;
	glm::vec3 mRotation;
	float mRotAngle;
};

#endif // #define __MODEL_H