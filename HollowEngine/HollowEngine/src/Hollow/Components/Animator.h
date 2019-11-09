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
		HOLLOW_API void Init();
		HOLLOW_API void Clear();
		HOLLOW_API void Serialize(rapidjson::Value::Object data);
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		HOLLOW_API void DebugDisplay();

	public:
		std::vector<Bone*> mBones;
		std::vector<std::unordered_map<std::string, std::pair<bool, AnimationData>>> mAnimationData;
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