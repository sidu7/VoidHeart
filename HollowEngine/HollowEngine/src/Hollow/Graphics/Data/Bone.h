#pragma once
#include "AnimationData.h"

namespace Hollow
{
	struct Bone
	{
		unsigned int mIndex;
		unsigned int mParentIndex;
		glm::mat4 mTransformation;
		glm::mat4 mCurrentTransformation;
		glm::mat4 mOffset;
		bool isAnimated;
		std::string mName;
		std::unordered_map<std::string, AnimationData> mAnimations;
	};

	struct BoneData
	{
		glm::ivec4 mBoneIndex;
		glm::vec4 mBoneWeights;

		inline BoneData(int index, float weight) : mBoneIndex(glm::ivec4(index)), mBoneWeights(glm::vec4(weight)) { }

		inline void AddBone(int index, float weight)
		{
			if (mBoneWeights[0] == 0.0f) { mBoneIndex[0] = index; mBoneWeights[0] = weight; }
			if (mBoneWeights[1] == 0.0f) { mBoneIndex[1] = index; mBoneWeights[1] = weight; }
			if (mBoneWeights[2] == 0.0f) { mBoneIndex[2] = index; mBoneWeights[2] = weight; }
			if (mBoneWeights[3] == 0.0f) { mBoneIndex[3] = index; mBoneWeights[3] = weight; }
		}
	};

}