#pragma once
#include "AbstractShader.h"


class SimpleGPShader : public AbstractShader
{
	std::string fshader =
R"(#version 430 core

in vec3 viewSpacePos;
out vec4 color;

uniform vec4 vColor;

uniform bool useFog;
const vec4 fogColor = vec4(0.5, 0.5, 0.5, 1.0);
const float fogDensity = 0.35;

void main(void)
{
	color = vColor;

	if (useFog)
	{
		float dist = length(viewSpacePos);
		float fogFactor = 1.0 / exp(dist * fogDensity);
		fogFactor = clamp(fogFactor, 0.0, 1.0);
		color = mix(fogColor, color, fogFactor);
	}
}

)";




	std::string vshader =
R"(#version 430 core                          
in vec3 position;

uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;

out vec3 viewSpacePos;

void main()
{
	viewSpacePos = (matView*matModel*vec4(position,1.0)).xyz;
	gl_Position = matProjection*matView*matModel*vec4(position, 1.0);
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


