#pragma once
#include "AbstractShader.h"

//blinn phong
class ModelShaderBlinnPhong : public AbstractShader
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
	float coneAngle;
};
uniform int structLightSize;
const int MAXLIGHTS = 32;
uniform Light structLight[MAXLIGHTS];

vec4 texColor;
vec3 normalizedNormal;

vec4 MakeLightPoint(vec3 vAmbient,vec3 surfaceToLight,vec3 surfaceToCamera,float attenuation,Light light)
{
	vec3 diffuseColor= vec3(0, 0, 0);
	float diffuseFactor = max(0.0,dot(normalizedNormal, surfaceToLight));
	if (diffuseFactor > 0) 
	{
		diffuseColor = vec3(light.diffuseColor * light.diffuseIntensity * diffuseFactor);
	}

	float specularFactor = 0.0;
	if(diffuseFactor > 0.0)
		specularFactor = pow(max(0.0, dot(normalizedNormal, normalize(surfaceToLight + surfaceToCamera))), shininess); //this is the only difference between Phong and Blinn Phong
	vec3 specularColor = vec3(specularFactor *light.specularIntensity* light.specularColor);


	vec3 linearColor =vAmbient + attenuation*(diffuseColor+specularColor);

	vec3 gamma = vec3(1.0/2.2);

	return vec4(pow(linearColor, gamma),texColor.w);//temp test code (really poor quality);
}

void main(void)
{
	texColor =texture(texture_diffuse, TexCoord);
	normalizedNormal = normalize(normal); //safety to counter interpolation    

	color=vec4(0,0,0,0);
	for(int i =0;i<structLightSize;++i)
	{
		if(structLight[i].type==0)//IK ifs are bad in a shader >=)
		{
			  vec4 vAmbient = vec4(structLight[i].ambientColor * structLight[i].ambientIntensity, 1);
			  float diffuseFactor = dot(normalizedNormal, -structLight[i].direction);               

			  vec4 diffuseColor;
			  if (diffuseFactor > 0) 
			  {
				  diffuseColor = vec4(structLight[i].diffuseColor * structLight[i].diffuseIntensity * diffuseFactor, 1);
			  }
			  else 
			  {
				  diffuseColor = vec4(0, 0, 0, 1);
			  }

			  color+= texture(texture_diffuse, TexCoord)*(vAmbient + diffuseColor);
		}
		else if(structLight[i].type==1)
		{
			vec3 vAmbient = structLight[i].ambientColor * structLight[i].ambientIntensity *texColor.xyz;	     
			vec3 surfaceToLight = normalize(structLight[i].position - vertices);
			vec3 surfaceToCamera = normalize(cameraPosition - vertices);      

			//attenuation
			float distanceToLight = length(structLight[i].position-vertices);
			float attenuation = 1.0/(1.0+structLight[i].attenuation*pow(distanceToLight,2));

			color+= MakeLightPoint(vAmbient,surfaceToLight,surfaceToCamera,attenuation,structLight[i]);
		}
		else if(structLight[i].type==2)
		{

			vec3 vAmbient = structLight[i].ambientColor * structLight[i].ambientIntensity *texColor.xyz;	     
			vec3 surfaceToLight = normalize(structLight[i].position - vertices);
			vec3 surfaceToCamera = normalize(cameraPosition - vertices); 
     
			//attenuation
			float distanceToLight = length(structLight[i].position-vertices);
			float attenuation = 1.0/(1.0+structLight[i].attenuation*pow(distanceToLight,2));
		
			float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(-structLight[i].direction))));
			if(lightToSurfaceAngle < structLight[i].coneAngle)
			{
				color+= MakeLightPoint(vAmbient,surfaceToLight,surfaceToCamera,attenuation,structLight[i]);
			}		
		}
	}
	color=(color/structLightSize)*vColor;// simple personalized way to counter overly white colors
	
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


