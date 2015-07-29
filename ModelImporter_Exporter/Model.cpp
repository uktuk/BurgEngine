#include "Common.h"
#include "Model.h"



GLint TextureFromFile(const char* path, std::string &directory);

Model::Model(GLchar* path)
: mMeshes(NULL)
, mIndices(NULL)
, mTexturesLoaded(NULL)
, mDirectory(NULL)
, mRoot(nullptr)
, mBones(NULL)
, mPosition(glm::vec3(0.0f, 0.0f, 0.0f))
, mRotation(glm::vec3(1.0f, 0.0f, 0.0f))
, mRotAngle(0.0f)
{
}

Model::~Model()
{
	Unload();
}

void Model::Unload()
{
	// Unload Meshes
	for (u32 i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i]->Destroy();
		SafeDelete(mMeshes[i]);
	}
	mMeshes.clear();

	// Unload Textures
	for (u32 i = 0; i < mTexturesLoaded.size(); ++i)
	{
		SafeDelete(mTexturesLoaded[i]);
	}
	mTexturesLoaded.clear();
}