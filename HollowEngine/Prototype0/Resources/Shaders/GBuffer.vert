#version 430
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in ivec4 boneIndex;
layout (location = 4) in vec4 boneWeights;

const int MAX_BONES = 100;

uniform mat4 Model;
uniform mat4 NormalTr;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 BoneTransforms[MAX_BONES];
uniform bool isAnimated;

out vec4 fragmentPosition;
out vec3 normalVector;
out vec2 texCoords;
out float cameraDepth;

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

	fragmentPosition = Model * BoneTr * vec4(position, 1.0);
	// May need to take the inverse transpose of the model matrix
	vec4 normalL = BoneTr * vec4(normal,0.0);
	normalVector = (normalL * NormalTr).xyz;
	// Most likely want to do this on the CPU, send like model matrix
	//normalVector = normal;

	// Send texture coordinates
	texCoords = textureCoordinates;

	cameraDepth = vec4(View * vec4(fragmentPosition.xyz, 1.0)).z;

	//gl_Position = Projection * View * vec4(fragmentPosition, 1.0);
	gl_Position = Projection * View * vec4(fragmentPosition.xyz, 1.0);
}