#ifndef __DIRECTIONAL_LIGHT_H
#define __DIRECTIONAL_LIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "Shader.h"

typedef glm::vec3 vect3;

class DirLight
{
public:
	DirLight(Shader &shader, vect3 &direction, vect3 &ambient, vect3 &diffuse, vect3 &specular, float strength);
	DirLight(Shader &shader, vect3 &direction, vect3 &colorRGB, float strength);
	void Update();

public:
	vect3 mDirection;
	vect3 mAmbient;
	vect3 mDiffuse;
	vect3 mSpecular;
	float mStrength;

private:
	Shader &mShader;
};


DirLight::DirLight(Shader &shader, vect3 &direction, vect3 &ambient, vect3 &diffuse, vect3 &specular, float strength)
	: mShader(shader)
	, mDirection(direction)
	, mAmbient(ambient)
	, mDiffuse(diffuse)
	, mSpecular(specular)
	, mStrength(strength)	
{	
	mAmbient *= mStrength;
	mDiffuse *= mStrength;
	mSpecular *= mStrength;
}

DirLight::DirLight(Shader &shader, vect3 &direction, vect3 &colorRGB, float strength)
:mShader(shader)
, mDirection(direction)
, mAmbient((colorRGB / 255.0f)* vect3(0.1f, 0.1f, 0.1f))
, mDiffuse(colorRGB / 255.0f)
, mSpecular(colorRGB / 255.0f)
, mStrength(strength)
{
	mAmbient *= strength;
	mDiffuse *= strength;
	mSpecular *= strength;
}

void DirLight::Update()
{
	glUniform3f(mShader.GetUniformLocation("dirLight.direction"), mDirection.x, mDirection.y, mDirection.z);
	glUniform3f(mShader.GetUniformLocation("dirLight.ambient"), mAmbient.x, mAmbient.y, mAmbient.z);
	glUniform3f(mShader.GetUniformLocation("dirLight.diffuse"), mDiffuse.x, mDiffuse.y, mDiffuse.z);
	glUniform3f(mShader.GetUniformLocation("dirLight.specular"), mSpecular.x, mSpecular.y, mSpecular.z);
}

#endif // #define __DIRECTIONAL_LIGHT_H