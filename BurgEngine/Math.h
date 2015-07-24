#ifndef __MATH_H
#define __MATH_H
#include <glm/glm.hpp>
#include <fstream>

namespace BMath
{
	// ================================================================	
	//  Function to read in a vector 3 from a file seperated by spaces
	// ================================================================= 
	glm::vec3 ReadVec3(std::ifstream& inFile, char *buffer, int size);
	

	// =================================================================
	//  Function to read in a vector 2 from a file seperated by spaces
	// =================================================================
	glm::vec2 ReadVec2(std::ifstream& inFile, char *buffer, int size);
}

#endif // #define __MATH_H