#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 aInstancedMatrix;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
	gl_Position = Projection * View * aInstancedMatrix * vec4(aPos,1.0);
}