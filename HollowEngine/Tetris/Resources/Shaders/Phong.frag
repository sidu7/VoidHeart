#version 430

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;

uniform sampler2D texture_diffuse;

in vec3 normalVector;
in vec3 fragmentPosition;
in vec2 texCoord;

out vec4 color;

void main()
{
	// Get normal, light, and view vectors
	vec3 N = normalize(normalVector);
	vec3 L = normalize(lightPosition - fragmentPosition);
	vec3 V = normalize(viewPosition - fragmentPosition);
	vec3 H = normalize(L+V);

	// Calculate clamped dot products
	float NL = max(dot(N, L), 0.0);
	float NV = max(dot(N, V), 0.0);
	float HN = max(dot(H, N), 0.0);

	vec3 Ka = vec3(0.2);
	//vec3 Kd = diffuseColor;
	vec3 Kd = texture(texture_diffuse,texCoord).xyz;
	vec3 Ks = specularColor;

	// Calculate ambient specular and diffuse colors
	vec3 ambient = Ka*Kd;
	vec3 diffuse = Kd*NL;
	vec3 specular = Ks*pow(HN, shininess);

	// Result
	vec3 result = diffuse + ambient + specular;
	color = vec4(result, 1.0);
}