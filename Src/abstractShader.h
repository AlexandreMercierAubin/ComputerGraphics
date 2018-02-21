#pragma once
#include <string>

class AbstractShader
{
	public:
		virtual std::string getShader() 
		{
			return shader;
		}
	private:
		std::string shader="";
};
