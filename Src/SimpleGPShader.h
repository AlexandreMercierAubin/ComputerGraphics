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
uniform mat4 matProjection;
uniform mat4 matModel;


void main()
{
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


