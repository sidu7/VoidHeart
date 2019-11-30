#version 430 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D UITexture;
uniform vec3 UIColor;
uniform bool hasTexture;

void main()
{
	if(hasTexture)
	{
		FragColor = texture(UITexture,TexCoords);
	}
	else
	{
		FragColor = vec4(UIColor,1.0);
	}
}