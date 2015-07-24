#ifndef TO_SHADER_H
#define TO_SHADER_H
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
	// The program ID
	GLuint Program;
	// Constructor reads and builds the shader
	Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath, GLuint numPointLights);
	// Use the program
	void Use();
	GLint GetUniformLocation(const char* string);
};

#endif // #define TO_SHADER_H