#version 430 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D blur;

void main()
{             
    vec3 sceneColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(blur, TexCoords);
	sceneColor += bloomColor.rgb; // additive blending
	FragColor = vec4(sceneColor, 1.0);
}