#pragma once
#include <stdio.h>
#include <math.h>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include<fstream>
#include <experimental/filesystem>
#include "abstractShader.h"
#include<sstream>

namespace Core
{
	class Shader_Loader
	{
	private:
		std::string ReadShader(abstractShader &absShader);
		GLuint CreateShader(GLenum shaderType, std::string source, std::string shadername);
	
	public:
		Shader_Loader(void);
		~Shader_Loader(void);

		GLuint CreateProgram(abstractShader &absVertexShader, abstractShader &absFragmentShader);
	};
}


