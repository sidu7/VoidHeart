#version 430 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D UITexture;
uniform vec3 UIColor;
uniform int hasTexture;
uniform float alpha;

void main()
{
	FragColor = vec4(UIColor, alpha) + hasTexture*texture(UITexture, TexCoords);
	// This is a pretty hacky way to deal with transparency for now
	if(FragColor.a < 0.1)
		discard;
//	if(hasTexture)
//	{
//		FragColor = texture(UITexture,TexCoords);
//	}
//	else
//	{
//		FragColor = vec4(UIColor, alpha);
//	}
}