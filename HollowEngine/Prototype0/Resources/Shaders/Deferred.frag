#version 430

#define PI 3.14159265

out vec4 color;

in vec2 texCoords;

// G-Buffer
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecular;

uniform vec3 viewPosition;
uniform vec3 lightPosition;

uniform int displayMode;

void main()
{
	// Get data from G-Buffer
	vec4 fragmentPosition = texture(gPosition, texCoords);
	vec3 normalVector = texture(gNormal, texCoords).rgb;
	vec3 diffuseColor = texture(gDiffuse, texCoords).rgb;
	vec3 specularColor = texture(gSpecular, texCoords).rgb;
	float shininess = texture(gSpecular, texCoords).a;

	// Get normal, light, and view vectors
	vec3 N = normalize(normalVector);
	vec3 L = normalize(lightPosition - fragmentPosition.xyz);
	vec3 V = normalize(viewPosition - fragmentPosition.xyz);
	vec3 H = normalize(L+V);

	// Calculate clamped dot products
	float NL = max(dot(N, L), 0.0);
	float NV = max(dot(N, V), 0.0);
	float HN = max(dot(H, N), 0.0);

	vec3 Ka = vec3(0.2);
	vec3 Kd = diffuseColor;
	vec3 Ks = specularColor;

	// Calculate ambient specular and diffuse colors
	vec3 ambient = Ka*Kd;
	vec3 diffuse = Kd*NL;
	vec3 specular = Ks*pow(HN, shininess);

	// Result
	vec3 result = diffuse + ambient + specular;

	color = vec4(result, 1.0);
	
	if(displayMode == 1)
	{
		color = vec4(fragmentPosition.xyz, 1.0);
	}
	else if(displayMode == 2)
	{
		color = vec4(normalVector, 1.0);
	}
	else if(displayMode == 3)
	{
		color = vec4(diffuseColor, 1.0);
	}
	else if(displayMode == 4)
	{
		color = vec4(specularColor, 1.0);
	}
}