#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 4) in ivec4 boneIndex;
layout (location = 5) in vec4 boneWeights;

const int MAX_BONES = 100;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 BoneTransforms[MAX_BONES];
uniform bool isAnimated;

out vec4 position;

void main()
{
	mat4 BoneTr = mat4(1.0);
	if(isAnimated)
	{
		BoneTr =  BoneTransforms[boneIndex[0]] * boneWeights[0];
		BoneTr += BoneTransforms[boneIndex[1]] * boneWeights[1];
		BoneTr += BoneTransforms[boneIndex[2]] * boneWeights[2];
		BoneTr += BoneTransforms[boneIndex[3]] * boneWeights[3];
	}

    gl_Position = Projection * View * Model * BoneTr * vec4(aPos, 1.0);
    position = gl_Position;
}