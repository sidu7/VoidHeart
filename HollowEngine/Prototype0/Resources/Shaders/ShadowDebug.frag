#version 430

out vec4 color;

in vec2 texCoords;

uniform sampler2D shadowMap;

void main()
{
	color = texture(shadowMap, texCoords);
}