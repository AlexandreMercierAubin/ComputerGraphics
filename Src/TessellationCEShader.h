#pragma once
#include "AbstractShader.h"


class TexShader : public AbstractShader
{
	std::string fshader =//evaluation shader
R"(#version 430 core
layout(triangles, equal_spacing, cw) in;

uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;

in vec2 tcs[];

out vec2 texCoord;

float grapher3d(vec4 P)
{
	// func001
	float y = -0.55 + exp(sin(P.x * 2)*sin(P.z*0.2))*0.6 * exp(sin(P.z * 2) * sin(P.x*0.2))*0.6;
	return y;
}

void main(void)
{
	vec3 pos = (gl_TessCoord.x*gl_in[0].gl_Position.xyz + gl_TessCoord.y*gl_in[1].gl_Position.xyz + gl_TessCoord.z*gl_in[2].gl_Position.xyz);

	pos.y = grapher3d(vec4(pos, 1.0));
	gl_Position = matProjection*matView*matModel* vec4(pos,1.0);

	vec2 tc0 = gl_TessCoord.x * tcs[0];
	vec2 tc1 = gl_TessCoord.y * tcs[1];
	vec2 tc2 = gl_TessCoord.z * tcs[2];
	texCoord = tc0 + tc1 + tc2;
}

)";




	std::string vshader =//control shader
R"(#version 430 core                          
in vec2 tc[];

out vec2 tcs[];

void main()
{
	gl_TessLevelInner[0] = 15.0; //15
	gl_TessLevelOuter[0] = 20.0; //20
	gl_TessLevelOuter[1] = 20.0; //20
	gl_TessLevelOuter[2] = 20.0; //20

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	tcs[gl_InvocationID] = tc[gl_InvocationID];
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


