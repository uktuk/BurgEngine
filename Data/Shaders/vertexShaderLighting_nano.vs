#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
	gl_Position = projectionMat * viewMat * modelMat * vec4(position, 1.0f);

	// TODO:: calculate the normal matrix on the CPU and send it to the shaders via a uniform before drawing
	Normal = mat3(transpose(inverse(modelMat))) * normal;
	FragPos = vec3(modelMat * vec4(position, 1.0f));
	TexCoords = texCoords;
};