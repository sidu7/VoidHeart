#version 430

uniform vec3 diffuseColor;

in vec3 fragmentPosition;

out vec4 color;

void main()
{
	// Result
	color = vec4(diffuseColor, 1.0);
}