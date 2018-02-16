#pragma once
#include <string>

class abstractShader
{
	public:
		virtual std::string getShader() 
		{
			return shader;
		}
	private:
		std::string shader="";
};
