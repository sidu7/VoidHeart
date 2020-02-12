#version 430 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 bloom;

in vec2 TexCoord;

uniform sampler2D DiffuseTexture;
uniform bool HasTexture;
uniform vec3 DiffuseColor;

void main()
{
	if(HasTexture)
	{
		FragColor = texture(DiffuseTexture,TexCoord);
	}
	else
	{
		FragColor = vec4(DiffuseColor,1.0f);
	}
	bloom = FragColor;
}