#version 430

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinates;

out vec4 fragmentPosition;
out vec3 normalVector;
out vec2 texCoords;
out float cameraDepth;

void main()
{
	fragmentPosition = Model * vec4(position, 1.0);
	// May need to take the inverse transpose of the model matrix
	normalVector = transpose(inverse(mat3(Model))) * normal;
	// Most likely want to do this on the CPU, send like model matrix
	//normalVector = normal;

	// Send texture coordinates
	texCoords = textureCoordinates;

	cameraDepth = vec4(View * vec4(fragmentPosition.xyz, 1.0)).z;

	//gl_Position = Projection * View * vec4(fragmentPosition, 1.0);
	gl_Position = Projection * View * vec4(fragmentPosition.xyz, 1.0);
}