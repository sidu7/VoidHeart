#version 430 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 bloom;

in vec2 TexCoord;
in vec3 ParticleColor;

uniform sampler2D DiffuseTexture;
uniform bool HasTexture;

void main()
{
	if(HasTexture)
	{
		FragColor = texture(DiffuseTexture,TexCoord);
	}
	else
	{
		FragColor = vec4(ParticleColor,1.0f);
	}
	bloom = FragColor;
}