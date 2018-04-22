#pragma once
#include "AbstractShader.h"


class TessellationShader : public AbstractShader
{
	std::string fshader =
R"(#version 430 core

in vec2 texCoord;
out vec4 color;

uniform sampler2D text;
uniform vec4 vColor;

void main(void)
{
	color = texture(text, texCoord)*vColor;
}

)";




	std::string vshader =
R"(#version 430 core                          
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;

out vec2 TexCoord;

void main()
{
	gl_Position =  matProjection*matView*matModel*(vec4(position, 1.0));
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


