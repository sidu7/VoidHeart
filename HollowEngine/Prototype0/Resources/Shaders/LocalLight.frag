#version 430

out vec4 color;

in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecular;

uniform vec3 viewPosition;
uniform vec3 lightPosition;

uniform float lightRadius;
//uniform float lightLinear;
//uniform float lightQuadratic;

// Pass screen size
uniform vec2 screenSize;

void main()
{
	// Use glFragCoord to get texture coordinates
	vec2 texCoords = gl_FragCoord.xy / screenSize;
	// Check if pixel is within light radius and should be lit
	vec3 fragmentPosition = texture(gPosition, texCoords).rgb;
	float distance = length(lightPosition - fragmentPosition);
	vec3 result = vec3(0.0, 0.0, 0.0);
	if(distance < lightRadius)
	{
		// Get data from G-Buffer
		vec3 normalVector = texture(gNormal, texCoords).rgb;
		vec3 diffuseColor = texture(gDiffuse, texCoords).rgb;
		vec3 specularColor = texture(gSpecular, texCoords).rgb;
		float shininess = texture(gSpecular, texCoords).a;

		// Get normal, light, and view vectors
		vec3 N = normalize(normalVector);
		vec3 L = normalize(lightPosition - fragmentPosition);
		vec3 V = normalize(viewPosition - fragmentPosition);
		vec3 H = normalize(L+V);

		// Calculate clamped dot products
		float NL = max(dot(N, L), 0.0);
		float HN = max(dot(H, N), 0.0);

		vec3 Kd = diffuseColor;
		vec3 Ks = specularColor;

		// Calculate ambient specular and diffuse colors
		vec3 diffuse = Kd*NL;
		vec3 specular = Ks*pow(HN, shininess);

		// Calulate light attenuation
		//float attenuation = 1.0 / (1.0 + lightLinear * distance + lightQuadratic * distance * distance);
		float attenuation = clamp(1.0 - distance*distance / (lightRadius * lightRadius), 0.0, 1.0);
		attenuation *= attenuation;

		// Result
		result = diffuse + specular;
		result *= attenuation; 
	}
	color = vec4(result, 1.0);
}