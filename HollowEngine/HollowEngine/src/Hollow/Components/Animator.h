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
		std::unordered_map<std::string,std::pair<double,double>> mAnimations;
		std::vector<glm::mat4> mBoneTransformations;
		std::string mRunningState;
		float mScaleFactor;
		double mRunTime;
		bool mLoopingAnimation;
	};
}