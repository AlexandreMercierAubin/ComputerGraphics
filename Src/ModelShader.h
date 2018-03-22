#pragma once
#include "AbstractShader.h"


class ModelShader : public AbstractShader
{
	std::string fshader =
R"(#version 430 core


in vec3 normal;
in vec2 TexCoord;
in vec3 vertices;
out vec4 color;

uniform sampler2D texture_diffuse;
uniform vec4 vColor;
uniform vec3 cameraPosition;
uniform float shininess;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	vec3 direction;
	vec3 position;
};

uniform Light structLight;

void main(void)
{
	vec4 vAmbient = vec4(structLight.color * structLight.ambientIntensity, 1);	     
    vec3 surfaceToLight = normalize(structLight.position - vertices);
    vec3 surfaceToCamera = normalize(cameraPosition - vertices);          

	vec4 diffuseColor;
	float diffuseFactor = dot(normalize(normal), -structLight.direction);
	if (diffuseFactor > 0) 
	{
		diffuseColor = vec4(structLight.color * structLight.diffuseIntensity * diffuseFactor, 1);
	}
	else 
	{
		diffuseColor = vec4(0, 0, 0, 1);
	}

	float specularFactor = 0.0;
    if(diffuseFactor > 0.0)
        specularFactor = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), shininess);
    vec4 specularColor = vec4(specularFactor *structLight.diffuseIntensity* structLight.color,1.0);//change diffuseIntensity for specularIntensity


	color= texture(texture_diffuse, TexCoord)*(vAmbient + diffuseColor+specularColor)*vColor;
}

)";



	std::string vshader =
R"(#version 430 core      
                    
in vec3 in_position;
in vec3 in_normal;
in vec2 texCoord;
 
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;

out vec3 normal;
out vec3 vertices;
out vec2 TexCoord;

void main()
{
	vertices = (matModel*vec4(in_position,1.0)).xyz;
	gl_Position =  matProjection*matView*matModel*(vec4(in_position, 1.0));
	normal = (matModel*(vec4(in_normal, 1))).xyz;
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


