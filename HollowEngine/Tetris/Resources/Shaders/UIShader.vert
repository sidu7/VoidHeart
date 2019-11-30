#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;

out vec2 TexCoords;

uniform mat4 Projection;
uniform mat4 Model;

void main()
{
	gl_Position = Projection * Model * vec4(aPos,1.0);
	TexCoords = aTex;
}