#pragma once
#include "abstractShader.h"

class QuadShader : public AbstractShader
{
	std::string fshader =
R"(#version 430 core

uniform sampler2D text;
in vec2 texCoords;
out vec4 color;

void main()
{
	color = texture2D(text, texCoords);
	//color = vec4(texCoords.x, texCoords.y, 0.0, 1.0);
}

)";

	std::string vshader =
R"(#version 430 core

layout(location = 0) in vec3 in_pos;
uniform mat4 matView;
uniform mat4 matPerspective;

out vec2 texCoords;

void main()
{
	gl_Position = matPerspective * matView * vec4(in_pos, 1.0);
	texCoords = vec2((in_pos.x + 0.25) * 2, (in_pos.y - 0.25) * -2);
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
