#include "Common.h"
#include "Model.h"



GLint TextureFromFile(const char* path, std::string &directory);

Model::Model(GLchar* path)
	: mPosition(glm::vec3(0.0f, 0.0f, 0.0f))
	, mRotation(glm::vec3(1.0f, 0.0f, 0.0f))
	, mRotAngle(0.0f)
{
	LoadModel(path);
}

Model::~Model()
{
	Unload();
}

void Model::Draw(Shader& shader)
{
	glm::mat4 model;
	model = glm::translate(model, mPosition);
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
	model = glm::rotate(model, mRotAngle, mRotation);
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));

	for (GLuint i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i]->Draw(shader);
	}
}

void Model::Translate(glm::vec3 transBy)
{
	mPosition += transBy;
}

void Model::SetPosition(glm::vec3 transTo)
{
	mPosition = transTo;
}

void Model::Rotate(float angle, glm::vec3 rot)
{
	mRotation = rot;
	mRotAngle = angle;
}

void Model::LoadModel(std::string path)
{
	// Retrieve directory from path and process nodes
	mDirectory = path.substr(0, path.find_last_of('/'));

	std::ifstream inFile;
	inFile.open(path, std::ifstream::in | std::ifstream::binary);
	char buffer[256];

	// Read total number of meshes
	inFile.getline(buffer, 256);
	uint totalMeshNum = std::stoi(buffer);
	uint totalVertNum = 0;
	uint totalIndicesNum = 0;
	uint totalTexturesNum = 0;
	
	// For each mesh in scene
	for (uint meshIndex = 0; meshIndex < totalMeshNum; ++meshIndex)
	{
		// Read number of vertices
		inFile.getline(buffer, 256);
		totalVertNum = std::stoi(buffer);

		Vertex* vertices = new Vertex[totalVertNum];  // Allocate space for array of new vertices    // Deleted in Mesh deconstructor
		Vertex* vertexIter = vertices;				  // Iterator to traverse through the new array of verts

		for (uint vertIndex = 0; vertIndex < totalVertNum; ++vertIndex)
		{
			// Read position data
			vertexIter->Position = BMath::ReadVec3(inFile, buffer, 256);
			// Read normal data
			vertexIter->Normal = BMath::ReadVec3(inFile, buffer, 256);
			// Read UV data
			vertexIter->TexCoords = BMath::ReadVec2(inFile, buffer, 256);
			++vertexIter;
		}

		// Read number of indices
		inFile.getline(buffer, 256);
		totalIndicesNum = std::stoi(buffer);

		uint *indices = new uint[totalIndicesNum]; // Deleted in Mesh deconstructor
		uint *indexIter = indices;

		for (uint i = 0; i < totalIndicesNum; i += 3)
		{
			inFile.getline(buffer, 256);
			indexIter[0] = std::stoi(buffer);
			inFile.getline(buffer, 256);
			indexIter[1] = std::stoi(buffer);
			inFile.getline(buffer, 256);
			indexIter[2] = std::stoi(buffer);

			indexIter += 3;
		}

		// Read Number of textures
		glm::vec2 texVec = BMath::ReadVec2(inFile, buffer, 256);
		u8 numTextures = ((int)texVec.x + (int)texVec.y);
		Texture* textures = new Texture[numTextures];  // allocate enough space for both types of textures  // Deleted in Mesh deconstructor
		Texture* texIter = textures;

		// Read in Diffuse textures
		for (uint i = 0; i < (uint)texVec.x; ++i)
		{
			inFile.getline(buffer, 256);
			std::vector<Texture*> diffuseMaps;
			LoadMaterialTextures(diffuseMaps, buffer, aiTextureType_DIFFUSE, "texture_diffuse");
			for (uint j = 0; j < diffuseMaps.size(); ++j)
			{
				*texIter = *diffuseMaps[j];
				++texIter;
			}
		}

		// Read in Specular textures
		for (uint i = 0; i < (uint)texVec.y; ++i)
		{
			inFile.getline(buffer, 256);
			std::vector<Texture*> specularMaps;
			LoadMaterialTextures(specularMaps, buffer, aiTextureType_SPECULAR, "texture_specular");
			for (uint j = 0; j < specularMaps.size(); ++j)
			{
				*texIter = *specularMaps[j];
				++texIter;
			}
		}
		

		Mesh *mesh = new Mesh();
		MeshBuilder::GenerateMesh(*mesh, vertices, totalVertNum, indices, totalIndicesNum, textures, (int)texVec.x + (int)texVec.y);
		mMeshes.push_back(mesh);

		vertexIter = nullptr;
		indexIter = nullptr;
		texIter = nullptr;
	}

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

void Model::LoadMaterialTextures(std::vector<Texture*>& textureData, char* file, aiTextureType type, std::string typeName)
{
	GLboolean skip = false;

	// Check if texture has already been loaded
	for (GLuint j = 0; j < mTexturesLoaded.size(); ++j)
	{
		if (strcmp(mTexturesLoaded[j]->path.C_Str(), file) == 0)
		{
			textureData.push_back(mTexturesLoaded[j]);
			skip = true;
			break;
		}
	}

	// Otherwise, load the texture
	if (!skip)
	{
		// TODO::create memory management system for resource sharing
		Texture* texture = new Texture();
		texture->id = TextureFromFile(file, mDirectory);
		texture->type = typeName;
		texture->path = file;
		textureData.push_back(texture);
		mTexturesLoaded.push_back(texture);
	}
}

GLint TextureFromFile(const char* path, std::string &directory)
{
	//Generate texture ID and load texture data 
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	printf("Image loading: %s\n", SOIL_last_result());
	
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}