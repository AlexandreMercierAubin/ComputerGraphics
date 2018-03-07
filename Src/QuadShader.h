#pragma once
#include "abstractShader.h"

class QuadShader : public AbstractShader
{
	std::string fshader =
R"(#version 430 core

in vec2 texCoords;
in vec4 Color;
out vec4 color;
uniform sampler2D tex;

void main()
{
	color = texture(tex, texCoords) * Color;
}

)";

	std::string vshader =
R"(#version 430 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_texCoords;
uniform mat4 matView;
uniform mat4 matPerspective;

out vec2 texCoords;
out vec4 Color;

void main()
{
	gl_Position = matPerspective * matView * vec4(in_pos, 1.0);
	texCoords = in_texCoords;
	Color = in_color;
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
