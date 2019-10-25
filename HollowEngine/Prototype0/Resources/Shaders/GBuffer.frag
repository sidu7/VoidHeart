#version 430

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec3 gDiffuse;
layout (location = 3) out vec4 gSpecular;

in vec4 fragmentPosition;
in vec3 normalVector;
in vec2 texCoords;
in float cameraDepth;

uniform vec3 diffuseColor;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform vec3 specularColor;
uniform float shininess;

void main()
{
	// Store the position in the G-Buffer
	gPosition = fragmentPosition;

	// Store the normal in the G-Buffer
	gNormal = vec4(normalize(normalVector), gl_FragDepth);

	// Store diffuse color in the G-Buffer
	//gDiffuse = diffuseColor;
	gDiffuse = diffuseColor + texture(diffuseTexture, texCoords).rgb;

	// Store specular color in the G-Buffer
	gSpecular = vec4(specularColor, shininess);
}