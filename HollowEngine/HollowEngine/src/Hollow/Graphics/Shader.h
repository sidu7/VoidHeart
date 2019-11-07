#pragma once

#include <sstream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Hollow {
	class Shader
	{
	public:
		HOLLOW_API Shader(const GLchar* pVertexPath, const GLchar* pFragmentPath);
		HOLLOW_API Shader(const GLchar* pComputePath);
		HOLLOW_API ~Shader();
		 
		HOLLOW_API void Use();
		HOLLOW_API void Unbind();
		HOLLOW_API void DispatchCompute(const unsigned int group_x, const unsigned int group_y, const unsigned int group_z) const;
		HOLLOW_API void SetInt(const std::string& name, int value) const;
		HOLLOW_API void SetUInt(const std::string& name, unsigned int value) const;
		HOLLOW_API void SetFloat(const std::string& name, float value) const;
		HOLLOW_API void SetVec2(const std::string& name, const glm::vec2& value) const;
		HOLLOW_API void SetVec3(const std::string& name, const glm::vec3& value) const;
		HOLLOW_API void SetVec3(const std::string& name, float x, float y, float z) const;
		HOLLOW_API void SetMat4(const std::string& name, const glm::mat4& mat) const;
		HOLLOW_API void SetUniformBlock(const std::string& name, const int bindPoint) const;
		HOLLOW_API void SetInputUniformImage(std::string name, unsigned int textureId, unsigned int imageUnit, unsigned int channels) const;
		HOLLOW_API void SetOutputUniformImage(std::string name, unsigned int textureId, unsigned int imageUnit, unsigned int channels) const;

	public:
		GLuint mProgram;

	private:
		std::stringstream CopyFileToStringStream(const GLchar* pFilePath);
		GLuint CreateAndCompileShader(GLenum shaderType, const GLchar* shaderSource);
		void CreateAndCompileProgram(GLuint vertexId, GLuint fragmentId);
	};
}