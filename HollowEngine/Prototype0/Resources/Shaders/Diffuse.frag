#version 430

uniform vec3 diffuseColor;
uniform vec3 viewPosition;

in vec3 fragmentPosition;
in vec3 normalVector;

out vec4 color;

void main()
{
	// Result
	vec3 result = diffuseColor * max(0.0, dot(normalVector, viewPosition - fragmentPosition));
	color = vec4(result, 1.0);
}