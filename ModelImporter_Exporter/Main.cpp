// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <fstream>

#include "Common.h"
#include "Model.h"
#include "Bone.h"


enum WriteMode
{
	wRaw,
	wDebug,
};

void LoadModel(char* path, WriteMode writeMode, Model& model);

void main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Model model;
	LoadModel("../Data/Models/Nanosuit/Nanosuit.obj", wDebug, model);
}

void LoadModel(char* path, WriteMode writeMode, Model& model)
{
	bool dFlag = (writeMode == wDebug);
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	if (!scene)
	{
		printf("Failed to load model - %s\n", importer.GetErrorString());
	}
	assert(scene, "Failed to load model - %s\n", importer.GetErrorString());

	// Write back to the path given to us
	std::string pathStr(path);
	size_t result = pathStr.find_last_of('/');
	pathStr = pathStr.substr(0, result+1);
	if (dFlag)
		pathStr.append("OutputModel_Debug.txt");
	else
		pathStr.append("OutputModel.txt");

	std::ofstream outFile;
	// outFile.open("../Data/Models/OutPutModel.txt", std::ios::out | std::ios::binary);
	outFile.open(pathStr, std::ios::out | std::ios::binary);
	
	if (dFlag)
		outFile << "Number of Meshes: ";
	outFile << scene->mNumMeshes << std::endl;

	if (scene->HasMeshes())
	{
		for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			aiMesh* aiMesh = scene->mMeshes[meshIndex];
			Mesh* mesh = new Mesh();

			// Vertices
			if (dFlag)
			outFile << "Num Vertices: ";
			outFile << aiMesh->mNumVertices << std::endl;
			for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i)
			{
				if (dFlag)
				outFile << "----------------------------------------" << std::endl;
				// Write position
				if (dFlag)
				outFile << "Position: ";
				outFile << aiMesh->mVertices[i].x << " "
						<< aiMesh->mVertices[i].y << " "
						<< aiMesh->mVertices[i].z << std::endl;

				// Write normals
				if (dFlag)
				outFile << "Normals: ";
				outFile << aiMesh->mNormals[i].x << " "
						<< aiMesh->mNormals[i].y << " "
						<< aiMesh->mNormals[i].z << std::endl;

				// Write UVs
				if (dFlag)
				outFile << "UV: ";
				outFile << aiMesh->mTextureCoords[0][i].x << " "
						<< aiMesh->mTextureCoords[0][i].y << std::endl;
			}

			// Indices
			if (dFlag)
			outFile << "Num Indices: ";
			outFile << aiMesh->mNumFaces * 3 << std::endl;
			for (unsigned int j = 0; j < aiMesh->mNumFaces; ++j)
			{
				for (unsigned int i = 0; i < aiMesh->mFaces[j].mNumIndices; ++i)
				{
					outFile << aiMesh->mFaces[j].mIndices[i] << std::endl;
				}
			}

			if (aiMesh->HasBones())
			{
				VertexWeights& vertexWeights = mesh->GetVertexWeights();
				vertexWeights.resize(aiMesh->mNumVertices);

				for (u32 i = 0; i < aiMesh->mNumBones; ++i)
				{
					aiBone* aiBone = aiMesh->mBones[i];
					u32 boneIndex = 0;

					// see if we have already added this bone
					auto iter = model.mBoneIndexMap.find(aiBone->mName.C_Str());
					if (iter != model.mBoneIndexMap.end())
					{
						boneIndex = iter->second;
					}
					else
					{
						// Gather data for bone information
						boneIndex = model.mBones.size();
						Bone* newBone = new Bone();
						ASSERT(aiBone->mName.length > 0, "Bone %d doesn't have a name", boneIndex);
						newBone->name = aiBone->mName.C_Str();
						newBone->index = boneIndex;

						//TODO: check that this actually works
						newBone->offsetTransform = *(glm::mat4x4*)&aiBone->mOffsetMatrix;
						
						// Push data back into model for later writing
						model.mBones.push_back(newBone);
						model.mBoneIndexMap.insert(std::make_pair(aiBone->mName.C_Str(), boneIndex));
					}

					for (u32 j = 0; j < aiBone->mNumWeights; ++j)
					{
						aiVertexWeight& aiVertexweight = aiBone->mWeights[j];
						BoneWeight weight;
						weight.boneIndex = boneIndex;
						weight.weight = aiVertexweight.mWeight;
						vertexWeights[aiVertexweight.mVertexId].push_back(weight);
					}
				}
			}

			unsigned int numDiffuseTextures = 0;
			unsigned int numSpecularTextures = 0;
			if (scene->HasMaterials())
			{				
				
				// Get diffuse textures
				aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];
				unsigned int textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
				for (unsigned int j = 0; j < textureCount; ++j)
				{
					aiString texturePath;
					if (material->GetTexture(aiTextureType_DIFFUSE, j, &texturePath) == AI_SUCCESS)
					{
						++numDiffuseTextures;
					}						
				}

				textureCount = material->GetTextureCount(aiTextureType_SPECULAR);
				for (unsigned int j = 0; j < textureCount; ++j)
				{
					aiString texturePath;
					if (material->GetTexture(aiTextureType_SPECULAR, j, &texturePath) == AI_SUCCESS)
					{
						++numSpecularTextures;
					}
				}			

				// Write number of diffuse/specular textures
				if (dFlag)
				outFile << "[Diff Textures] [Spec Textures]" << std::endl;
				outFile << numDiffuseTextures << " " << numSpecularTextures << std::endl;

				if (dFlag)
				outFile << "Diffuse Textures" << std::endl;

				// Write Diffuse texture paths
				material = scene->mMaterials[aiMesh->mMaterialIndex];
				textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
				for (unsigned int j = 0; j < textureCount; ++j)
				{
					aiString texturePath;
					if (material->GetTexture(aiTextureType_DIFFUSE, j, &texturePath) == AI_SUCCESS)
					{
						outFile << texturePath.C_Str() << std::endl;
					}
				}

				if (dFlag)
				outFile << "Specular Textures" << std::endl;

				// Write Specular texture paths
				for (unsigned int j = 0; j < textureCount; ++j)
				{
					aiString texturePath;
					if (material->GetTexture(aiTextureType_SPECULAR, j, &texturePath) == AI_SUCCESS)
					{
						outFile << texturePath.C_Str() << std::endl;
					}
				}
			}

			delete mesh;
		}
		u32 numBones = model.mBones.size();
		if (dFlag)
		outFile << "Num Bones: ";
		outFile << numBones << std::endl;
		for (u32 i = 0; i < numBones; ++i)
		{
			// name
			outFile << model.mBones[i]->name << std::endl;

			// parent index
			outFile << model.mBones[i]->parentIndex << std::endl;

			// children indices
			u32 childrenNum = model.mBones[i]->children.size();
			outFile << childrenNum << std::endl;
			for (u32 j = 0; j < childrenNum; ++j)
			{
				outFile << model.mBones[i]->children[j]->index << std::endl;
			}
		}


		for (u32 i = 0; i < numBones; ++i)
		{
			delete model.mBones[i];
		}
	}


	
	printf("Done.\n");
	outFile.close();
	getchar();
}



/*
void LoadModel(char* path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	if (!scene)
	{
		printf("Failed to load model - %s\n", importer.GetErrorString());
	}
	assert(scene, "Failed to load model - %s\n", importer.GetErrorString());

	std::ofstream outFile;
	outFile.open("../Data/Models/OutPutModel.txt", std::ios::out | std::ios::binary);

	outFile << scene->mNumMeshes << std::endl;

	if (scene->HasMeshes())
	{
		for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			aiMesh* aiMesh = scene->mMeshes[meshIndex];

			// Vertices
			outFile << aiMesh->mNumVertices << std::endl;
			for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i)
			{
				// Write position
				outFile << aiMesh->mVertices[i].x << " "
					<< aiMesh->mVertices[i].y << " "
					<< aiMesh->mVertices[i].z << std::endl;

				// Write normals
				outFile << aiMesh->mNormals[i].x << " "
					<< aiMesh->mNormals[i].y << " "
					<< aiMesh->mNormals[i].z << std::endl;

				// Write UVs
				outFile << aiMesh->mTextureCoords[0][i].x << " "
					<< aiMesh->mTextureCoords[0][i].y << std::endl;
			}

			// Indices
			outFile << aiMesh->mNumFaces * 3 << std::endl;
			for (unsigned int j = 0; j < aiMesh->mNumFaces; ++j)
			{
				for (unsigned int i = 0; i < aiMesh->mFaces[j].mNumIndices; ++i)
				{
					outFile << aiMesh->mFaces[j].mIndices[i] << std::endl;
				}
			}

			unsigned int numDiffuseTextures = 0;
			unsigned int numSpecularTextures = 0;
			if (scene->HasMaterials())
			{

				// Get diffuse textures
				aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];
				unsigned int textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
				for (unsigned int j = 0; j < textureCount; ++j)
				{
					aiString texturePath;
					if (material->GetTexture(aiTextureType_DIFFUSE, j, &texturePath) == AI_SUCCESS)
					{
						++numDiffuseTextures;
					}
				}

				textureCount = material->GetTextureCount(aiTextureType_SPECULAR);
				for (unsigned int j = 0; j < textureCount; ++j)
				{
					aiString texturePath;
					if (material->GetTexture(aiTextureType_SPECULAR, j, &texturePath) == AI_SUCCESS)
					{
						++numSpecularTextures;
					}
				}


				// Write number of diffuse/specular textures
				outFile << numDiffuseTextures << " " << numSpecularTextures << std::endl;

				// Write Diffuse texture paths
				material = scene->mMaterials[aiMesh->mMaterialIndex];
				textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
				for (unsigned int j = 0; j < textureCount; ++j)
				{
					aiString texturePath;
					if (material->GetTexture(aiTextureType_DIFFUSE, j, &texturePath) == AI_SUCCESS)
					{
						outFile << texturePath.C_Str() << std::endl;
					}
				}

				// Write Specular texture paths
				for (unsigned int j = 0; j < textureCount; ++j)
				{
					aiString texturePath;
					if (material->GetTexture(aiTextureType_SPECULAR, j, &texturePath) == AI_SUCCESS)
					{
						outFile << texturePath.C_Str() << std::endl;
					}
				}
			}
		}
	}


	printf("Done.\n");
	outFile.close();
	getchar();
}
*/