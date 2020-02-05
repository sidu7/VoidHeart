#version 430 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 textureCoordinates;

struct ParticleData
{
	mat4 mModel;
	vec4 more_data;
};

layout(std430, binding = 3) buffer ModelBlock
{
	ParticleData data[];
};

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

out vec2 TexCoord;

void main()
{
	if(data[gl_InstanceID].more_data.x < 0.0001f)
	{
		gl_Position = vec4(0.0f);
	}
	else
	{
		gl_Position = Projection * View * Model * data[gl_InstanceID].mModel * vec4(position,1.0);
	}
	TexCoord = textureCoordinates;
}