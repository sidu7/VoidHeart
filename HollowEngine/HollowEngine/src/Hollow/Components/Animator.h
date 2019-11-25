#pragma once
#include "Component.h"
#include "Hollow/Graphics/Data/AnimationData.h"

namespace Hollow
{
	struct Bone;

	class Animator : public Component
	{
		REGISTERCOMPONENT(Animator);
	public:
		HOLLOW_API void Init() override;
		HOLLOW_API void Clear() override;
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
		HOLLOW_API void DebugDisplay() override;

	private:
		std::string mBoneDataPath;
		std::vector<std::pair<std::string, std::string>> mAnimationsDataPath;
		
	public:
		std::vector<Bone*> mBones;
		std::vector<std::unordered_map<std::string, std::pair<bool, AnimationData>>> mAnimationData; // Corresponds to bones vector, contains each bones animation data
		std::unordered_map<std::string,std::pair<double,double>> mAnimations; // Animation Name -> {ticks per sec,animation duration}
		std::vector<glm::mat4> mBoneTransformations;
		std::unordered_map<std::string, glm::mat4> mSkeleton; // Stores current transformation matrix for each bone
		std::string mCurrentState;
		std::string mPreviousState;
		float mScaleFactor; // Used to scale the model to unit length
		double mCurrentRunTime;
		double mPreviousRunTime;
		bool mLoopingAnimation;
		bool mBlending;
		float mBlendTime;
		float mBlendFactor;
	};
}