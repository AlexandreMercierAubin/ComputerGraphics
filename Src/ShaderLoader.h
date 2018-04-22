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
#include "AbstractShader.h"
#include<sstream>

namespace Core
{
	class ShaderLoader
	{
	private:
		GLuint CreateShader(GLenum shaderType, std::string source, std::string shadername);
		
	public:
		ShaderLoader(void);
		~ShaderLoader(void);

		GLuint CreateProgram(AbstractShader &absShader);
		GLuint CreateProgramTess(AbstractShader &absShader, AbstractShader &absShaderCE);
	};
}


