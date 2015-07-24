#include "Common.h"
#include "Shader.h"


Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, GLuint numPointLights)
{
	// Retrieve the vertex/fragment source code from filepath
	char* vertexCode;
	char* fragmentCode;
	FILE* vShaderFile;
	FILE* fShaderFile;

	
	vShaderFile = fopen(vertexPath, "rb");
	fShaderFile = fopen(fragmentPath, "rb");
	if (vShaderFile == nullptr || fShaderFile == nullptr)
	{
		printf("Error opening shader file locations\n");
		return;
	}

	// Get size of files
	fseek(vShaderFile, 0, SEEK_END);
	int vSize = ftell(vShaderFile);
	rewind(vShaderFile);

	fseek(fShaderFile, 0, SEEK_END);
	int fSize = ftell(fShaderFile);
	rewind(fShaderFile);

	// Allocate memory for files
	vertexCode = new char[vSize+1];
	vertexCode[vSize] = '\0';

	fragmentCode = new char[fSize+1];
	fragmentCode[fSize] = '\0';

	// Read in files
	unsigned int result = fread(vertexCode, 1, vSize, vShaderFile);
	if (result != vSize)
	{
		printf("Error reading vertex code\n");
		return;
	}

	result = fread(fragmentCode, 1, fSize, fShaderFile);
	if (result != fSize)
	{
		printf("Error reading fragment code\n");
		return;
	}
	fclose(vShaderFile);
	fclose(fShaderFile);

	// array[43] is number of point lights to create space for
	if (fragmentCode[43] == '@')
	{
		char *num = new char[2];
		_itoa(numPointLights, num, 10);
		fragmentCode[43] = num[0];

		delete[] num;
		num = nullptr;
	}


	const GLchar* vShaderCode = vertexCode;
	//const GLchar* vShaderCode = "\
	//								#version 330 core\
	//									layout(location = 0) in vec3 position;\
	//										layout(location = 1) in vec3 color;\
	//												out vec3 ourColor;";
	const GLchar* fShaderCode = fragmentCode;

	// Compile shaders
	GLint success;
	GLchar infoLog[512];

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR: vertex shader compilation failed\n%s\n", infoLog);
	}

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR: fragment shader compilation failed\n%s\n", infoLog);
	}

	// Shader Program
	Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, fragment);
	glLinkProgram(Program);

	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
		printf("ERROR: shader program failed to link\n%s\n", infoLog);
	}

	// Cleanup
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	delete[] vertexCode;
	delete[] fragmentCode;
}

void Shader::Use()
{
	glUseProgram(Program);
}

GLint Shader::GetUniformLocation(const char* string)
{
	return glGetUniformLocation(Program, string);
}