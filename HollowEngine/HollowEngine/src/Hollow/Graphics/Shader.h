#pragma once

#include <sstream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Hollow {
	class HOLLOW_API Shader
	{
	public:
		Shader(const GLchar* pVertexPath, const GLchar* pFragmentPath);
		Shader(const GLchar* pComputePath);
		~Shader();

		void Use();
		void Unbind();
		void DispatchCompute(const int group_x, const int group_y, const int group_z) const;
		void SetInt(const std::string& name, int value) const;
		void SetUInt(const std::string& name, unsigned int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetVec2(const std::string& name, const glm::vec2& value) const;
		void SetVec3(const std::string& name, const glm::vec3& value) const;
		void SetVec3(const std::string& name, float x, float y, float z) const;
		void SetMat4(const std::string& name, const glm::mat4& mat) const;
		void SetUniformBlock(const std::string& name, const int bindPoint) const;

	public:
		GLuint mProgram;

	private:
		std::stringstream CopyFileToStringStream(const GLchar* pFilePath);
		GLuint CreateAndCompileShader(GLenum shaderType, const GLchar* shaderSource);
		void CreateAndCompileProgram(GLuint vertexId, GLuint fragmentId);
	};
}