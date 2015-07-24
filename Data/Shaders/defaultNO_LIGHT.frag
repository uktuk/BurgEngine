#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{    
    color = vec4(texture(texture_diffuse1, TexCoords));
	//color = vec4(TexCoords.xy, 1.0f, 1.0f);
}