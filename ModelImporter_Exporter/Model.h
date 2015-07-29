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
#include <vector>
#include "Mesh.h"
#include "Math.h"
#include "Bone.h"

// Class declaration
class Model
{
public:
	Model()
	{}


	Model(char* path);
	~Model();
	void Unload();

public:
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