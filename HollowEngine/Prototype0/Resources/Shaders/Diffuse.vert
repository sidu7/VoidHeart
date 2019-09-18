#version 430

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

layout (location = 0) in vec3 position;

out vec3 fragmentPosition;

void main()
{
	fragmentPosition = vec3(Model * vec4(position, 1.0));
	gl_Position = vec4(fragmentPosition, 1.0);//Projection * View * vec4(fragmentPosition, 1.0);
}