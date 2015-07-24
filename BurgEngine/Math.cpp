#include "Common.h"
#include "Math.h"
#include <string>



glm::vec3 BMath::ReadVec3(std::ifstream& inFile, char *buffer, int size)
{
	inFile.getline(buffer, size);
	char *pch;
	glm::vec3 vec;
	pch = strtok(buffer, " ");
	vec.x = std::stof(pch);
	pch = strtok(NULL, " ");
	vec.y = std::stof(pch);
	pch = strtok(NULL, " ");
	vec.z = std::stof(pch);
	return vec;
}

glm::vec2 BMath::ReadVec2(std::ifstream& inFile, char *buffer, int size)
{
	inFile.getline(buffer, size);
	char *pch;
	glm::vec2 vec;
	pch = strtok(buffer, " ");
	vec.x = std::stof(pch);
	pch = strtok(NULL, " ");
	vec.y = std::stof(pch);
	return vec;
}