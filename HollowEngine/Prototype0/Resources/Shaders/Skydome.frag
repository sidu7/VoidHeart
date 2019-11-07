#version 430

#define PI 3.141592

in vec3 fragmentPosition;

uniform sampler2D skydome;
uniform float exposure;
uniform float contrast;

layout (location = 0) out vec4 color;

void main()
{
	vec3 v = normalize(fragmentPosition.xyz);
	vec2 uv = vec2(0.5 - (atan(v.z, v.x)/(2.0*PI)), acos(-v.y)/PI); 
	//color = vec4(texture(skydome, uv).rgb, 1.0f);
	
	// Tone map and gamma correction
	color = textureLod(skydome, uv, 0.0);
	vec4 eC = exposure*color;
	color = eC / (eC + vec4(1.0));
	color = pow(color, vec4(contrast/2.2));
}