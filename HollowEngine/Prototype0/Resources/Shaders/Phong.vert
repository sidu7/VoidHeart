#version 430

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fragmentPosition;
out vec3 normalVector;

void main()
{
	fragmentPosition = vec3(Model * vec4(position, 1.0));
	// May need to take the inverse transpose of the model matrix
	normalVector = mat3(transpose(inverse(Model))) * normal;
	// Most likely want to do this on the CPU, send like model matrix
	//normalVector = normal;

	gl_Position = Projection * View * vec4(fragmentPosition, 1.0);
}