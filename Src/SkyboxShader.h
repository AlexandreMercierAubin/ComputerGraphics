#pragma once
#include "AbstractShader.h"


class SkyboxShader : public AbstractShader
{
	std::string fshader =
R"(#version 430 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;
uniform vec4 vColor;

uniform bool useFog;
const vec4 fogColor = vec4(0.5, 0.5, 0.5, 1.0);
const float fogDensity = 0.35;

void main()
{
	if (useFog)
		color = fogColor;
	else
		color = texture(skybox, TexCoords)*vColor;
}

)";




	std::string vshader =
R"(#version 430 core 
in vec3 position;
uniform mat4 matProjection;
uniform mat4 matView;
out vec3 TexCoords;

void main() 
{
	vec4 pos = matProjection * matView * vec4(position, 1.0);
	gl_Position = pos.xyww;
	TexCoords = normalize(position);
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


