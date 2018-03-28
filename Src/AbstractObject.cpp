#pragma once
#include "AbstractObject.h"

AbstractObject::AbstractObject()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotationDegree = glm::vec3(0.0f, 0.0f, 0.0f);
	rotationQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


void AbstractObject::uniformColor(GLuint &program, glm::vec4 &uniformColor)
{
	GLuint vecCouleur = glGetUniformLocation(program, "vColor");
	glUniform4fv(vecCouleur, 1, &uniformColor[0]);
}

void AbstractObject::uniformCameraPosition(GLuint &program, glm::vec3 &uniformCameraPos) 
{
	GLuint vecCamera = glGetUniformLocation(program, "cameraPosition");
	glUniform4fv(vecCamera, 1, &uniformCameraPos[0]);
}


void AbstractObject::MatRotation(const GLuint &program, glm::mat4 &rotat, const glm::vec3 &r) // rotation matrix
{
	//x:pitch y:yaw z:roll

	rotat[0][0] = cosf(r.z)*cosf(r.y)-sinf(r.z)*sinf(r.x)*sinf(r.y);  rotat[0][1] = -sinf(r.z)*cosf(r.x);		rotat[0][2] = cosf(r.z)*sinf(r.y)+sinf(r.z)*sinf(r.x)*cosf(r.y);	rotat[0][3] = 0.0f;
	rotat[1][0] = sinf(r.z)*cosf(r.y)+cosf(r.z)*sinf(r.x)*sinf(r.y);  rotat[1][1] = cosf(r.z)*cosf(r.x);		rotat[1][2] = sinf(r.z)*sinf(r.y)-cosf(r.z)*sinf(r.x)*cosf(r.y);	rotat[1][3] = 0.0f;
	rotat[2][0] = -cosf(r.x)*sinf(r.y);								  rotat[2][1] = sinf(r.x);					rotat[2][2] = cosf(r.x)*cosf(r.y);									rotat[2][3] = 0.0f;
	rotat[3][0] = 0.0f;												  rotat[3][1] = 0.0f;						rotat[3][2] = 0.0f;													rotat[3][3] = 1.0f;

}

void AbstractObject::MatRotationDegree(const GLuint &program, glm::mat4 &rotat, const glm::vec3 &r)
{
	MatRotation(program, rotat, glm::vec3(glm::radians(r.x), glm::radians(r.y), glm::radians(r.z)));
}

void AbstractObject::MatTranslation(GLuint program, glm::mat4 &trans, const glm::vec3 &position) // matrice de translation
{

	trans[0][0] = 1.0f; trans[0][1] = 0.0f; trans[0][2] = 0.0f; trans[0][3] = 0.0f;
	trans[1][0] = 0.0f; trans[1][1] = 1.0f; trans[1][2] = 0.0f; trans[1][3] = 0.0f;
	trans[2][0] = 0.0f; trans[2][1] = 0.0f; trans[2][2] = 1.0f; trans[2][3] = 0.0f;
	trans[3][0] = position.x; trans[3][1] = position.y; trans[3][2] = position.z; trans[3][3] = 1.0f;
}

void AbstractObject::MatScale(GLuint program, glm::mat4 &scl, const glm::vec3 &scale) // matrice de translation
{

	scl[0][0] = scale.x;	  scl[0][1] = 0.0f;			scl[0][2] = 0.0f;			scl[0][3] = 0.0f;
	scl[1][0] = 0.0f;		  scl[1][1] = scale.y;		scl[1][2] = 0.0f;			scl[1][3] = 0.0f;
	scl[2][0] = 0.0f;		  scl[2][1] = 0.0f;			scl[2][2] = scale.z;		scl[2][3] = 0.0f;
	scl[3][0] = 0.0f;		  scl[3][1] = 0.0f;			scl[3][2] = 0.0f;			scl[3][3] = 1.0f;
}

void AbstractObject::MatRotationQuaternion(const GLuint &program, glm::mat4 &rotat, const glm::quat &r) //x:pitch y:yaw z:roll
{
	rotat[0][0] = 1-2*pow(r.y,2) - 2*pow(r.z, 2);  rotat[0][1] = 2*r.x*r.y - 2*r.w*r.z;					rotat[0][2] = 2*r.x*r.z+2*r.w*r.y;							rotat[0][3] = 0.0f;
	rotat[1][0] = 2*r.x*r.y+2*r.w*r.z;			   rotat[1][1] = 1 - 2*pow(r.x, 2) - 2*pow(r.z, 2);		rotat[1][2] = 2*r.y*r.z-2*r.w*r.x;							rotat[1][3] = 0.0f;
	rotat[2][0] = 2*r.x*r.z-2*r.w*r.y;			   rotat[2][1] = 2*r.y*r.z + 2*r.w*r.x;					rotat[2][2] = 1 - 2*pow(r.x, 2) - 2*pow(r.y, 2);			rotat[2][3] = 0.0f;
	rotat[3][0] = 0.0f;							   rotat[3][1] = 0.0f;									rotat[3][2] = 0.0f;											rotat[3][3] = 1.0f;

}

void AbstractObject::setPosition(glm::vec3 pos)
{
	position = pos;
}

void AbstractObject::addPosition(const glm::vec3 &pos)
{
	position += pos;
}

glm::vec3 AbstractObject::getPosition()
{
	return position;
}

void AbstractObject::setRotationDegree(glm::vec3 rot)
{
	rotationDegree = rot;
	rotationQuat = glm::quat(rot);
}

void AbstractObject::addRotationDegree(const glm::vec3 &rot)
{
	rotationDegree += rot;
	rotationQuat = glm::quat(rotationDegree);
}

glm::vec3 AbstractObject::getRotationDegree()
{
	return rotationDegree;
}

void AbstractObject::setRotationQuaternion(glm::quat quat)
{
	rotationQuat = quat;
	rotationDegree = glm::eulerAngles(quat);
}

void AbstractObject::addRotationQuaternion(const glm::quat &quat)
{
	rotationQuat = glm::quat(rotationQuat.w + quat.w, rotationQuat.x + quat.x, rotationQuat.y + quat.y, rotationQuat.z + quat.z);
	rotationDegree = glm::eulerAngles(rotationQuat);
}

glm::quat AbstractObject::getRotationQuaternion()
{
	return rotationQuat;
}

void AbstractObject::setScale(glm::vec3 scl)
{
	scale = scl;
}

void AbstractObject::addScale(const glm::vec3 &scl)
{
	scale += scl;
}

glm::vec3 AbstractObject::getScale()
{
	return scale;
}

void AbstractObject::setColor(glm::vec4 Color)
{
	color = Color;
}

glm::vec4 AbstractObject::getColor()
{
	return color;
}

void AbstractObject::uniformLight(GLuint &program, vector<Light*>lights)
{
	GLuint size = glGetUniformLocation(program, "structLightSize");
	glUniform1i(size, lights.size());

	for (unsigned int i = 0; i < lights.size(); ++i) 
	{
		string location = "structLight["+to_string(i)+"].ambientColor";
		glUniform3fv(glGetUniformLocation(program, location.c_str()) , 1, &(lights[i]->ambientColor[0])); //verbose, I know -_-

		location = "structLight[" + to_string(i) + "].ambientIntensity";
		glUniform1f(glGetUniformLocation(program,location.c_str()), lights[i]->ambientIntensity);

		location = "structLight[" + to_string(i) + "].diffuseColor";
		glUniform3fv(glGetUniformLocation(program, location.c_str()), 1, &(lights[i]->diffuseColor[0]));

		location = "structLight[" + to_string(i) + "].diffuseIntensity";
		glUniform1f(glGetUniformLocation(program,location.c_str()), lights[i]->diffuseIntensity);

		location = "structLight[" + to_string(i) + "].specularColor";
		glUniform3fv(glGetUniformLocation(program, location.c_str()),1, &(lights[i]->specularColor[0]));

		location = "structLight[" + to_string(i) + "].specularIntensity";
		glUniform1f(glGetUniformLocation(program, location.c_str()), lights[i]->specularIntensity);

		location = "structLight[" + to_string(i) + "].attenuation";
		glUniform1f(glGetUniformLocation(program, location.c_str()), lights[i]->attenuation);

		location = "structLight[" + to_string(i) + "].direction";
		glUniform3fv(glGetUniformLocation(program,location.c_str()), 1,&(lights[i]->direction[0]));

		location = "structLight[" + to_string(i) + "].position";
		glUniform3fv(glGetUniformLocation(program,location.c_str()), 1,&(lights[i]->position[0]));

		location = "structLight[" + to_string(i) + "].type";
		glUniform1i(glGetUniformLocation(program, location.c_str()), lights[i]->lightType);
	}
}

bool AbstractObject::isSelected()
{
	return selected;
}

void AbstractObject::setSelected(bool value)
{
	selected = value;
}

string AbstractObject::getName()
{
	return name;
}

