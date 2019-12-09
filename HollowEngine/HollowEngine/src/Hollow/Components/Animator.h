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
		/// <summary>
		/// Initializes the Animator Component.
		/// </summary>
		HOLLOW_API void Init() override;

		/// <summary>
		/// Clears the Animator component.
		/// </summary>
		HOLLOW_API void Clear() override;

		/// <summary>
		/// Serializes the specified data of the Animator component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current Animator component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

		/// <summary>
		/// To set the Debug Display for the Animator Component.
		/// </summary>
		HOLLOW_API void DebugDisplay() override;

	private:
		std::string mBoneDataPath;
		std::vector<std::pair<std::string, std::string>> mAnimationsDataPath;
		std::string mNewAnimationName;
		std::string mNewAnimationFile;
		
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