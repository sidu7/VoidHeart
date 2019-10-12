#version 330 core

out vec4 FragColor;

uniform sampler2D Texx;

void main()
{
	//FragColor = texture(Texx,gl_PointCoord);
	FragColor = vec4(0,1,0,1);
}