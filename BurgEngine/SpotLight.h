#ifndef __SPOT_LIGHT_H
#define __SPOT_LIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "Shader.h"

typedef glm::vec3 vect3;

class SpotLight
{
public:
	SpotLight(Shader &shader, vect3 &position, vect3 &direction, vect3 &ambient, vect3 &diffuse, vect3 &specular, float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f, float cutoff_rad = glm::radians(12.5f), float outercutoff_rad = glm::radians(15.0f));
	SpotLight(Shader &shader, vect3 &position, vect3 &direction, vect3 &colorRGB, float strength, float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f, float cutoff_rad = glm::radians(12.5f), float outercutoff_rad = glm::radians(15.0f));
	void Update();

	void SetPosDir(vect3 &position, vect3 &direction);

public:
	vect3 mPosition;
	vect3 mDirection;
	vect3 mAmbient;
	vect3 mDiffuse;
	vect3 mSpecular;

	float mStrength;

private:
	Shader &mShader;
	float mConstant;
	float mLinear;
	float mQuadratic;
	float mCutoff;
	float mOuterCutoff;

	float mCutoffCalc;
	float mOuterCutoffCalc;
};


SpotLight::SpotLight(Shader &shader, vect3 &position, vect3 &direction, vect3 &ambient, vect3 &diffuse, vect3 &specular, float constant, float linear, float quadratic, float cutoff_rad, float outercutoff_rad)
	: mShader(shader)
	, mPosition(position)
	, mDirection(direction)
	, mAmbient(ambient)
	, mDiffuse(diffuse)
	, mSpecular(specular)
	, mConstant(constant)
	, mLinear(linear)
	, mQuadratic(quadratic)
	, mCutoff(cutoff_rad)
	, mOuterCutoff(outercutoff_rad)
{
	mCutoffCalc = glm::cos(mCutoff);
	mOuterCutoffCalc = glm::cos(mOuterCutoff);
}

SpotLight::SpotLight(Shader &shader, vect3 &position, vect3 &direction, vect3 &colorRGB, float strength, float constant, float linear, float quadratic, float cutoff_rad, float outercutoff_rad)
	: mShader(shader)
	, mPosition(position)
	, mDirection(direction)
	, mAmbient((colorRGB / 255.0f)* vect3(0.1f, 0.1f, 0.1f))
	, mDiffuse(colorRGB / 255.0f)
	, mSpecular(colorRGB / 255.0f)
	, mStrength(strength)
	, mConstant(constant)
	, mLinear(linear)
	, mQuadratic(quadratic)
	, mCutoff(cutoff_rad)
	, mOuterCutoff(outercutoff_rad)
{
	mAmbient *= strength;
	mDiffuse *= strength;
	mSpecular *= strength;

	mCutoffCalc = glm::cos(mCutoff);
	mOuterCutoffCalc = glm::cos(mOuterCutoff);
}

void SpotLight::Update()
{
	glUniform3f(mShader.GetUniformLocation("spotLight.position"), mPosition.x, mPosition.y, mPosition.z);
	glUniform3f(mShader.GetUniformLocation("spotLight.direction"), mDirection.x, mDirection.y, mDirection.z);
	glUniform3f(mShader.GetUniformLocation("spotLight.ambient"), mAmbient.x, mAmbient.y, mAmbient.z);
	glUniform3f(mShader.GetUniformLocation("spotLight.diffuse"), mDiffuse.x, mDiffuse.y, mDiffuse.z);
	glUniform3f(mShader.GetUniformLocation("spotLight.specular"), mSpecular.x, mSpecular.y, mSpecular.z);
	glUniform1f(mShader.GetUniformLocation("spotLight.constant"), mConstant);
	glUniform1f(mShader.GetUniformLocation("spotLight.linear"), mLinear);
	glUniform1f(mShader.GetUniformLocation("spotLight.quadratic"), mQuadratic);
	glUniform1f(mShader.GetUniformLocation("spotLight.cutOff"), mCutoffCalc);
	glUniform1f(mShader.GetUniformLocation("spotLight.outerCutOff"), mOuterCutoffCalc);
}

void SpotLight::SetPosDir(vect3 &position, vect3 &direction)
{
	mPosition = position;
	mDirection = direction;
}


#endif // #define __SPOT_LIGHT_H