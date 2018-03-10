#pragma once
#include "AbstractShader.h"


class SimpleGPShader : public AbstractShader
{
	std::string fshader =
R"(#version 430 core

out vec4 color;

uniform vec4 vColor;

void main(void)
{
	color = vColor;
}

)";




	std::string vshader =
R"(#version 430 core                          
in vec3 position;

uniform mat4 matView;
uniform mat4 matTranslation;
uniform mat4 matPerspective;
uniform mat4 matRotation;
uniform mat4 matScale;


void main()
{
	gl_Position = matPerspective*matView*matTranslation*matRotation*matScale*vec4(position, 1.0);
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


