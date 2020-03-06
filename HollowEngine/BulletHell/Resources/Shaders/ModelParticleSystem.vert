#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
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
uniform mat4 NormalTr;

out vec2 TexCoord;
out vec3 ParticleColor;
out vec3 normalVec;
out vec3 worldPos;
out vec3 eyePos;

void main()
{
	if(data[gl_InstanceID].more_data.x < 0.0001f)
	{
		gl_Position = vec4(0.0f);
	}
	else
	{
		gl_Position = Projection * View * data[gl_InstanceID].mModel * vec4(position,1.0);
		worldPos = (data[gl_InstanceID].mModel * vec4(position,1.0)).xyz;

		normalVec = normal * mat3(NormalTr);

		eyePos = (inverse(View) * vec4(0,0,0,1)).xyz;
	}
		
	TexCoord = textureCoordinates;
	ParticleColor = data[gl_InstanceID].more_data.gba;
}