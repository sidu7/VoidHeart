#pragma once

#include "Hollow/Common.h"
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Hollow {
	class Shader
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Shader"/> class
		/// </summary>
		/// <param name="pVertexPath">Filepath to vertex shader</param>
		/// <param name="pFragmentPath">Filepath to fragment shader</param>
		HOLLOW_API Shader(const GLchar* pVertexPath, const GLchar* pFragmentPath);

		/// <summary>
		/// Initializes a new instance of the <see cref="Shader"/> class
		/// </summary>
		/// <param name="pComputePath">Filepath to compute shader</param>
		HOLLOW_API Shader(const GLchar* pComputePath);

		/// <summary>
		/// Destructor for an instance of the <see cref="Shader"/> class
		/// </summary>
		HOLLOW_API ~Shader();
		 
		/// <summary>
		/// Installs the shader as part of current rendering state
		/// </summary>
		HOLLOW_API void Use();

		/// <summary>
		/// Unbinds the shader from the current rendering state
		/// </summary>
		HOLLOW_API void Unbind();

		/// <summary>
		/// Dispatches a call to a compute shader
		/// </summary>
		/// <param name="group_x">The size of thread group in x dimension</param>
		/// <param name="group_y">The size of thread group in y dimension</param>
		/// <param name="group_z">The size of thread group in z dimension</param>
		HOLLOW_API void DispatchCompute(const unsigned int group_x, const unsigned int group_y, const unsigned int group_z) const;

		/// <summary>
		/// Sets an int in the shader program with the given name/key
		/// </summary>
		/// <param name="name">The varible name in the shader program</param>
		/// <param name="value">The int to send to the shader program</param>
		HOLLOW_API void SetInt(const std::string& name, int value) const;

		/// <summary>
		/// Sets an unsigned int in the shader program with the given name/key
		/// </summary>
		/// <param name="name">The varible name in the shader program</param>
		/// <param name="value">The unsigned int to send to the shader program</param>
		HOLLOW_API void SetUInt(const std::string& name, unsigned int value) const;

		/// <summary>
		/// Sets a float in the shader program with the given name/key
		/// </summary>
		/// <param name="name">The varible name in the shader program</param>
		/// <param name="value">The float to send to the shader program</param>
		HOLLOW_API void SetFloat(const std::string& name, float value) const;

		/// <summary>
		/// Sets a vec2 in the shader program with the given name/key
		/// </summary>
		/// <param name="name">The varible name in the shader program</param>
		/// <param name="value">The vec2 to send to the shader program</param>
		HOLLOW_API void SetVec2(const std::string& name, const glm::vec2& value) const;

		/// <summary>
		/// Sets a vec3 in the shader program with the given name/key
		/// </summary>
		/// <param name="name">The varible name in the shader program</param>
		/// <param name="value">The vec3 to send to the shader program</param>
		HOLLOW_API void SetVec3(const std::string& name, const glm::vec3& value) const;

		/// <summary>
		/// Sets a vec3 in the shader program with the given name/key
		/// </summary>
		/// <param name="name">The varible name in the shader program</param>
		/// <param name="x">The x value in the vec3</param>
		/// <param name="y">The y value in the vec3</param>
		/// <param name="z">The z value in the vec3</param>
		HOLLOW_API void SetVec3(const std::string& name, float x, float y, float z) const;

		HOLLOW_API void SetVec4(const std::string& name, glm::vec4& v) const;

		/// <summary>
		/// Sets a Mat4 in the shader program with the given name/key
		/// </summary>
		/// <param name="name">The varible name in the shader program</param>
		/// <param name="mat">The Mat4 to send to the shader program</param>
		HOLLOW_API void SetMat4(const std::string& name, const glm::mat4& mat) const;

		/// <summary>
		/// Assigns a binding point to an active uniform block
		/// </summary>
		/// <param name="name">The varible name in the shader program</param>
		/// <param name="bindPoint">The index of the active uniform block whose binding to assign</param>
		HOLLOW_API void SetUniformBlock(const std::string& name, const int bindPoint) const;

		/// <summary>
		/// Binds an input image for use in a compute shader
		/// </summary>
		/// <param name="name">The varible name in the shader program</param>
		/// <param name="textureId">The texture to bind to the image unit</param>
		/// <param name="imageUnit">The index of the image unit to which to bind the texture</param>
		/// <param name="channels">The number of channels in the texture</param>
		HOLLOW_API void SetInputUniformImage(std::string name, unsigned int textureId, unsigned int imageUnit, unsigned int channels) const;

		/// <summary>
		/// Binds an ouput image for use in a compute shader
		/// </summary>
		/// <param name="name">The varible name in the shader program</param>
		/// <param name="textureId">The texture to bind to the image unit</param>
		/// <param name="imageUnit">The index of the image unit to which to bind the texture</param>
		/// <param name="channels">The number of channels in the texture</param>
		HOLLOW_API void SetOutputUniformImage(std::string name, unsigned int textureId, unsigned int imageUnit, unsigned int channels) const;

	public:
		GLuint mProgram;

	private:
		std::stringstream CopyFileToStringStream(const GLchar* pFilePath);
		GLuint CreateAndCompileShader(GLenum shaderType, const GLchar* shaderSource);
		void CreateAndCompileProgram(GLuint vertexId, GLuint fragmentId);
	};
}