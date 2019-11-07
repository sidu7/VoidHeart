#version 430 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D blur;
uniform float exposure;
uniform float contrast;

void main()
{          
	const float gamma = 2.2;
	const float exposure = 0.5;
    vec3 sceneColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(blur, TexCoords).rgb;
	sceneColor += bloomColor.rgb; // additive blending

	vec3 eC = exposure*sceneColor;
	vec3 result = eC / (eC + vec3(1.0));
	result = pow(result, vec3(contrast/2.2));

	FragColor = vec4(result, 1.0);
}