#version 430 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 bloom;

in vec2 TexCoord;
in vec3 ParticleColor;
in vec3 normalVec;
in vec3 worldPos;
in vec3 eyePos;

uniform sampler2D DiffuseTexture;
uniform vec3 Specular;
uniform float Shininess;
uniform bool HasTexture;
uniform bool Lighting;
uniform vec3 LightPos;
uniform vec3 LightColor;

const float pi = 22.0f/7.0f;

void main()
{
	vec3 Kd = vec3(0.0f);
	if(HasTexture)
	{
		Kd = texture(DiffuseTexture,TexCoord).xyz;
	}
	else
	{
		Kd = ParticleColor;
	}
	if(Lighting)
	{
		vec3 lightVec = LightPos - worldPos;
		vec3 eyeVec = eyePos - worldPos;

		vec3 N = normalize(normalVec);
		vec3 L = normalize(lightVec);
		vec3 V = normalize(eyeVec);
		vec3 H = normalize(L + V);
		float NL = max(dot(L,N),0.0);
		float NH = max(dot(N,H),0.0);
		float LH = max(dot(L,H),0.0);
		
		vec3 Ks = Specular;
		
		float alp = Shininess;
		vec3 Ii = LightColor * pi;

		vec3 F = Ks + (vec3(1,1,1)-Ks)*pow((1-LH),5);
		float D = (alp+2) * pow(NH,alp) / (2*pi);

		vec3 BRDF = (Kd/pi) + ((F*D)/(4*pow(LH,2)));

		FragColor = vec4(Ii * NL * BRDF,1);
	}
	else
	{
		FragColor = vec4(Kd,1.0f);
	}
	
	bloom = FragColor;
}