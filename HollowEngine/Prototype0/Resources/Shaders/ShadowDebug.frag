#version 430

out vec4 color;

in vec2 texCoords;

uniform sampler2D shadowMap;
uniform float nearPlane;
uniform float farPlane;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{
	//float depthValue = texture(shadowMap, texCoords).r;
	//color = vec4(vec3(LinearizeDepth(depthValue) / farPlane), 1.0);
	color = vec4(texture(shadowMap, texCoords).r);
}