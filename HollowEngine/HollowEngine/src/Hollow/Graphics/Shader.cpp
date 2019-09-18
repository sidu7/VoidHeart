#include <hollowpch.h>

#include "Shader.h"

namespace Hollow {
	Shader::Shader(const GLchar* pVertexPath, const GLchar* pFragmentPath)
	{
		// Copy shader source files to strings
		std::stringstream vertexShaderStream = CopyFileToStringStream(pVertexPath);
		std::stringstream fragmentShaderStream = CopyFileToStringStream(pFragmentPath);

		// Convert string streams into GL character arrays
		std::string vertexShaderString = vertexShaderStream.str();
		std::string fragmentShaderString = fragmentShaderStream.str();
		const GLchar* vertexShaderCode = vertexShaderString.c_str();
		const GLchar* fragmentShaderCode = fragmentShaderString.c_str();

		// Compile shaders
		GLuint vertexId = CreateAndCompileShader(GL_VERTEX_SHADER, vertexShaderCode);
		GLuint fragmentId = CreateAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

		// Compile the shader program
		CreateAndCompileProgram(vertexId, fragmentId);

		// Delete linked shaders as they are no longer needed
		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);
	}

	Shader::Shader(const GLchar* pComputePath)
	{
		// Constructor for creating compute shader
		std::stringstream computeShaderStream = CopyFileToStringStream(pComputePath);
		std::string computeShaderString = computeShaderStream.str();
		const GLchar* computeShaderCode = computeShaderString.c_str();

		// Compile compute shader
		GLuint shaderID = CreateAndCompileShader(GL_COMPUTE_SHADER, computeShaderCode);

		// Create the shader program
		mProgram = glCreateProgram();
		glAttachShader(mProgram, shaderID);
		glLinkProgram(mProgram);

		// Delete linked shader
		glDeleteShader(shaderID);
	}

	void Shader::Use()
	{
		glUseProgram(mProgram);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	void Shader::SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(mProgram, name.c_str()), value);
	}

	void Shader::SetUInt(const std::string& name, unsigned int value) const
	{
		glUniform1ui(glGetUniformLocation(mProgram, name.c_str()), value);
	}

	void Shader::SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(mProgram, name.c_str()), value);
	}

	void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(mProgram, name.c_str()), 1, &value[0]);
	}

	void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(mProgram, name.c_str()), 1, &value[0]);
	}

	void Shader::SetVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(mProgram, name.c_str()), x, y, z);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(mProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	std::stringstream Shader::CopyFileToStringStream(const GLchar* pFilePath)
	{
		std::ifstream shaderFile;
		shaderFile.open(pFilePath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		return shaderStream;
	}

	GLuint Shader::CreateAndCompileShader(GLenum shaderType, const GLchar* shaderSource)
	{
		// Create the shader
		GLuint shaderId = glCreateShader(shaderType);

		// Compile the shader
		glShaderSource(shaderId, 1, &shaderSource, nullptr);
		glCompileShader(shaderId);

		// Print errors
		GLint success;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar errorLog[512];
			glGetShaderInfoLog(shaderId, 512, nullptr, errorLog);
			HW_CORE_ERROR("Error compiling shader!\n{0}", errorLog);
		}

		return shaderId;
	}

	void Shader::CreateAndCompileProgram(GLuint vertexId, GLuint fragmentId)
	{
		mProgram = glCreateProgram();
		glAttachShader(mProgram, vertexId);
		glAttachShader(mProgram, fragmentId);
		glLinkProgram(mProgram);

		// Print linking errors
		GLint success;
		glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar errorLog[512];
			glGetProgramInfoLog(mProgram, 512, nullptr, errorLog);
			HW_CORE_ERROR("Error linking shader program!\n{0}", errorLog);
		}
	}
}