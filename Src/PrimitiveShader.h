#pragma once
#include "abstractShader.h"

class PrimitiveShader : public AbstractShader
{
	std::string fshader =
R"(#version 430 core
out vec4 color;
uniform vec4 vColor;

void main()
{
	color = vColor;
}

)";

	std::string vshader =
R"(#version 430 core 
layout(location = 0) in vec3 position;

void main()
{
	gl_Position = vec4(position, 1.0);
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
