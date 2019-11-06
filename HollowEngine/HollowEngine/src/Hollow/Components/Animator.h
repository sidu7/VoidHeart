#pragma once
#include "Component.h"

namespace Hollow
{
	struct Bone;

	class HOLLOW_API Animator : public Component
	{
		REGISTERCOMPONENT(Animator);
	public:
		void Init();
		void Clear();
		void Serialize(rapidjson::Value::Object data);
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		void DebugDisplay();

	public:
		std::vector<Bone*> mBones;
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