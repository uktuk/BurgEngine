#ifndef __POINT_LIGHT_H
#define __POINT_LIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "Shader.h"

typedef glm::vec3 vect3;

class PointLight
{
public:
	PointLight(Shader &shader, vect3 &position, vect3 &ambient, vect3 &diffuse, vect3 &specular, float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);
	PointLight(Shader &shader, vect3 &position, vect3 &colorRGB, float strength, float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);
	void Update(int index = 0);

public:
	vect3 mPosition;
	vect3 mAmbient;
	vect3 mDiffuse;
	vect3 mSpecular;

	float mStrength;

private:
	Shader &mShader;
	float mConstant;
	float mLinear;
	float mQuadratic;
};


PointLight::PointLight(Shader &shader, vect3 &position, vect3 &ambient, vect3 &diffuse, vect3 &specular, float constant, float linear, float quadratic)
	: mShader(shader)
	, mPosition(position)
	, mAmbient(ambient)
	, mDiffuse(diffuse)
	, mSpecular(specular)
	, mConstant(constant)
	, mLinear(linear)
	, mQuadratic(quadratic)
{
}

PointLight::PointLight(Shader &shader, vect3 &position, vect3 &colorRGB, float strength, float constant, float linear, float quadratic)
	: mShader(shader)
	, mPosition(position)
	, mAmbient((colorRGB / 255.0f)* vect3(0.1f, 0.1f, 0.1f))
	, mDiffuse(colorRGB / 255.0f)
	, mSpecular(colorRGB / 255.0f)
	, mStrength(strength)
	, mConstant(constant)
	, mLinear(linear)
	, mQuadratic(quadratic)
{
	mAmbient *= strength;
	mDiffuse *= strength;
	mSpecular *= strength;
}

void PointLight::Update(int index)
{
	// create string to query from shader
	std::string str = std::to_string(index);

	glUniform3f(mShader.GetUniformLocation(("pointLights[" + str + "].position").c_str()), mPosition.x, mPosition.y, mPosition.z);
	glUniform3f(mShader.GetUniformLocation(("pointLights[" + str + "].ambient").c_str()), mAmbient.x, mAmbient.y, mAmbient.z);
	glUniform3f(mShader.GetUniformLocation(("pointLights[" + str + "].diffuse").c_str()), mDiffuse.x, mDiffuse.y, mDiffuse.z);
	glUniform3f(mShader.GetUniformLocation(("pointLights[" + str + "].specular").c_str()), mSpecular.x, mSpecular.y, mSpecular.z);
	glUniform1f(mShader.GetUniformLocation(("pointLights[" + str + "].constant").c_str()), mConstant);
	glUniform1f(mShader.GetUniformLocation(("pointLights[" + str + "].linear").c_str()), mLinear);
	glUniform1f(mShader.GetUniformLocation(("pointLights[" + str + "].quadratic").c_str()), mQuadratic);
}

#endif // #define __POINT_LIGHT_H