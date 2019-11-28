#version 430 core
layout(location = 0) in vec4 vertex; // <vec3 po, vec2 tex>

uniform mat4 Projection;

out vec2 TexCoords;
void main()
{
	gl_Position = Projection * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
}