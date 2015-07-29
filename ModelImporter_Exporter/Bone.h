#ifndef __BONE_H
#define __BONE_H

#include "Common.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Bone
{
	Bone()
	: parent(nullptr)
	, parentIndex(-1)
	, index(0)
	{}

	std::string name;
	Bone* parent;
	std::vector<Bone*> children;
	s32 parentIndex;  // need singed so root bone can be -1 ie. doesn't have a parent
	std::vector<u32> childrenIndex;

	glm::mat4x4 transform;
	glm::mat4x4 offsetTransform;

	u32 index;  // where you are in the bone array

};

#endif // define __BONE_H