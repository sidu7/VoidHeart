#include <hollowpch.h>
#include "GLCall.h"

namespace Hollow {
	
	void GLClearError()
	{
		//while (glGetError() != GL_NO_ERROR);
	}

	bool GLLogCall(const char* function, const char* file, int line)
	{
		return true;
		while (GLenum error = glGetError())
		{
			std::cout << "[OpenGl Error] (" << std::hex << error << std::dec << "):" << function << " " << file << ":" << line << std::endl;
			return false;
		}
		return true;
	}
}