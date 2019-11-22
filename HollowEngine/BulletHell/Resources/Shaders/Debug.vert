#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aColor;

uniform mat4 View, Projection, Model;

out vec3 vColor;

void main()
{
	gl_Position = Projection * View * Model * vec4(aPos,1.0);
	vColor = aColor;
}