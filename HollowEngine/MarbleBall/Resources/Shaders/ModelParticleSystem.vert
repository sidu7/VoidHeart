#version 430 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 textureCoordinates;

layout(std430, binding = 3) buffer ModelBlock
{
	mat4 mModel[];
};

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

out vec2 TexCoord;

void main()
{
	gl_Position = Projection * View * Model * mModel[gl_InstanceID] * vec4(position,1.0);
	TexCoord = textureCoordinates;
}