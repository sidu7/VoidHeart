#version 430 core

out vec4 FragColor;

uniform sampler2D Texx;
uniform int Type;

in float alpha;

void main()
{
	if (Type == 0)
	{
		FragColor = texture(Texx,gl_PointCoord);
		FragColor.a *= alpha;
	}
	else if (Type == 1)
	{
		FragColor = vec4(0,1,0,1);
	}
}