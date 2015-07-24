// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"
#include <fstream>

//#define debugLog

#define debPrint
#ifndef debugLog
#define debPrint
#endif

void LoadModel(char* path);
void DebugLoadModel(char* path);

void main()
{
	LoadModel("../Data/Models/Cute/NeatBoxSSpec.obj");
	//DebugLoadModel("../Data/Models/DefBox/NewCubeWOW.obj");	
}

void DebugLoadModel(char* path)
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
	
	outFile << "Number of Meshes: ";
	outFile << scene->mNumMeshes << std::endl;

	if (scene->HasMeshes())
	{
		for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			aiMesh* aiMesh = scene->mMeshes[meshIndex];

			// Vertices
			outFile << "Num Vertices: ";
			outFile << aiMesh->mNumVertices << std::endl;
			for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i)
			{
				outFile << "----------------------------------------" << std::endl;
				// Write position
				outFile << "Position: ";
				outFile << aiMesh->mVertices[i].x << " "
						<< aiMesh->mVertices[i].y << " "
						<< aiMesh->mVertices[i].z << std::endl;

				// Write normals
				outFile << "Normals: ";
				outFile << aiMesh->mNormals[i].x << " "
						<< aiMesh->mNormals[i].y << " "
						<< aiMesh->mNormals[i].z << std::endl;

				// Write UVs
				outFile << "UV: ";
				outFile << aiMesh->mTextureCoords[0][i].x << " "
						<< aiMesh->mTextureCoords[0][i].y << std::endl;
			}

			// Indices
			outFile << "Num Indices: ";
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