#pragma once
#include "AbstractShader.h"


class ModelShader : public AbstractShader
{
	std::string fshader =
R"(#version 430 core


in vec3 normal;
in vec2 TexCoord;
out vec4 color;

uniform sampler2D texture_diffuse;
uniform vec4 vColor;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	vec3 direction;
};

uniform Light structLight;

void main(void)
{
  vec4 vAmbient = vec4(structLight.color * structLight.ambientIntensity, 1);
  float diffuseFactor = dot(normalize(normal), -structLight.direction);               

  vec4 diffuseColor;
  if (diffuseFactor > 0) 
  {
	  diffuseColor = vec4(structLight.color * structLight.diffuseIntensity * diffuseFactor, 1);
  }
  else 
  {
	  diffuseColor = vec4(0, 0, 0, 1);
  }

  color= texture(texture_diffuse, TexCoord)*(vAmbient + diffuseColor)*vColor;
}

)";



	std::string vshader =
R"(#version 430 core      
                    
in vec3 position;
in vec3 in_normale;
in vec2 texCoord;
 
uniform mat4 matView;
uniform mat4 matTranslation;
uniform mat4 matPerspective;
uniform mat4 matRotation;
uniform mat4 matScale;

out vec3 normal;
out vec2 TexCoord;

void main()
{

  gl_Position =  matPerspective*matView*matTranslation*matRotation*matScale*(vec4(position, 1.0));
  normal = (matTranslation*matRotation*matScale*(vec4(in_normale, 0))).xyz;
  TexCoord = vec2(texCoord.x,texCoord.y);

}
)";

public:
	virtual std::string getFragmentShader()
	{
		return fshader;
	}
	
	virtual std::string getVertexShader()
	{
		return vshader;
	}

};


