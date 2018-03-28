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
	int type;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
	float attenuation;
	vec3 direction;
	vec3 position;
};
uniform int structLightSize;
const int MAXLIGHTS = 256;
uniform Light structLight[MAXLIGHTS];

void main(void)
{
	vec4 texColor =texture(texture_diffuse, TexCoord);

	color=vec4(0,0,0,0);
	for(int i =0;i<structLightSize;++i)
	{
		if(structLight[i].type==0)//IK ifs are bad in a shader >=)
		{
			  vec4 vAmbient = vec4(structLight[i].ambientColor * structLight[i].ambientIntensity, 1);
			  float diffuseFactor = dot(normalize(normal), -structLight[i].direction);               

			  vec4 diffuseColor;
			  if (diffuseFactor > 0) 
			  {
				  diffuseColor = vec4(structLight[i].diffuseColor * structLight[i].diffuseIntensity * diffuseFactor, 1);
			  }
			  else 
			  {
				  diffuseColor = vec4(0, 0, 0, 1);
			  }

			  color= texture(texture_diffuse, TexCoord)*(vAmbient + diffuseColor)*vColor;
		}
		else if(structLight[i].type==1)
		{
			vec3 vAmbient = structLight[i].ambientColor * structLight[i].ambientIntensity *texColor.xyz;	     
			vec3 surfaceToLight = normalize(structLight[i].position - vertices);
			vec3 surfaceToCamera = normalize(cameraPosition - vertices);          

			vec3 diffuseColor;
			float diffuseFactor = dot(normalize(normal), -structLight[i].direction);
			if (diffuseFactor > 0) 
			{
				diffuseColor = vec3(structLight[i].diffuseColor * structLight[i].diffuseIntensity * diffuseFactor);
			}
			else 
			{
				diffuseColor = vec3(0, 0, 0);
			}

			float specularFactor = 0.0;
			if(diffuseFactor > 0.0)
				specularFactor = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), shininess);
			vec3 specularColor = vec3(specularFactor *structLight[i].specularIntensity* structLight[i].specularColor);

			//attenuation
			float distanceToLight = length(structLight[i].position-vertices);
			float attenuation = 1.0/(1.0+structLight[i].attenuation*pow(distanceToLight,2));
		
			vec3 linearColor =vAmbient + attenuation*(diffuseColor+specularColor)*vColor.xyz;

			vec3 gamma = vec3(1.0/2.2);

			color= vec4(pow(linearColor, gamma),texColor.w);//temp test code (really poor quality)
	}
}

	
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
	normal = vec3(matModel*(vec4(in_normal, 0.0))).xyz;
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


