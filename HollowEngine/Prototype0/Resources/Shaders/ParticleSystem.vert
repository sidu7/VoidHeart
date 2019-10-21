#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 aInstancedMatrix;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform int Type;
uniform vec2 ScreenSize;
uniform float SpriteSize;

void main()
{
	if(Type == 0)
	{
		vec4 eyePos = View * Model * vec4(aPos,1.0);
		vec4 projVoxel = Projection * vec4(SpriteSize,SpriteSize,eyePos.z,eyePos.w);
		vec2 projSize = ScreenSize * projVoxel.xy / projVoxel.w;
		gl_PointSize = 0.25 * (projSize.x+projSize.y);
		gl_Position = Projection * eyePos;
	}
	else if (Type == 1)
	{
		gl_Position = Projection * View * aInstancedMatrix * vec4(aPos,1.0);
	}
}