#version 430 core

//layout (location = 0) in vec3 aPos;
//layout (location = 3) in mat4 aInstancedMatrix;

layout(std430, binding = 3) buffer PosBlock
{
	vec4 mPos[];
};

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform vec2 ScreenSize;
uniform float SpriteSize;

out float alpha;

void main()
{
		vec4 eyePos = View * Model * vec4(mPos[gl_VertexID].xyz,1.0);
		vec4 projVoxel = Projection * vec4(SpriteSize,SpriteSize,eyePos.z,eyePos.w);
		vec2 projSize = ScreenSize * projVoxel.xy / projVoxel.w;
		gl_PointSize = 0.25 * (projSize.x+projSize.y);
		gl_Position = Projection * eyePos;
		alpha = mPos[gl_VertexID].w;
}