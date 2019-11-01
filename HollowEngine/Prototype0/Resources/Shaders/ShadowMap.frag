#version 430 core

out vec4 FragColor;

in vec4 position;

void main()
{
	float depth = position.z / position.w;
	FragColor = vec4(depth, depth*depth, depth*depth*depth, depth*depth*depth*depth);
}