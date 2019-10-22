#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec4 position;

void main()
{
    gl_Position = Projection * View * Model * vec4(aPos, 1.0);
    position = gl_Position;
}