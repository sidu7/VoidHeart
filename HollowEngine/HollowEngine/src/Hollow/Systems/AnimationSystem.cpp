#include <hollowpch.h>
#include "AnimationSystem.h"

#include "Hollow/Components/Animator.h"
#include "Hollow/Components/StateMachine.h"

#include "Hollow/Graphics/Data/Bone.h"
#include "Hollow/Core/Data/StateData.h"

#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Managers/ThreadManager.h"

namespace Hollow
{
	AnimationSystem AnimationSystem::instance;
	
	void AnimationSystem::AddGameObject(GameObject* object)
	{
		CheckComponents<Animator,StateMachine>(object);
	}

	void AnimationSystem::Animate(unsigned int start, unsigned int end)
	{
		for (unsigned int i = start; i < end; ++i)
		{
			GameObject* gameobject = mGameObjects[i];
			Animator* animator = gameobject->GetComponent<Animator>();
			StateMachine* state = gameobject->GetComponent<StateMachine>();
			if (animator->mRunningState != state->mCurrentState->mName)
			{
				animator->mRunningState = state->mCurrentState->mName;
				animator->mLoopingAnimation = state->mCurrentState->mIsLoop;
				animator->mRunTime = FrameRateController::Instance().GetFrameTime();
			}
			animator->mRunTime += FrameRateController::Instance().GetFrameTime();
			
			auto ticks_duration = animator->mAnimations[animator->mRunningState];
			double TimeinTicks = animator->mRunTime * ticks_duration.first;
			double animationTime = TimeinTicks / ticks_duration.second;
			
			if (animationTime > 1.0 && !animator->mLoopingAnimation)
			{
				state->mNeedChangeState = true;
				break;
			}
			
			double timeFrame = fmod(TimeinTicks, ticks_duration.second);
			animator->mBoneTransformations.clear();
			std::string animationName = animator->mRunningState;
			std::vector<glm::mat4> currentTransformations;
			currentTransformations.reserve(animator->mBones.size());
			for (unsigned int j = 0; j < animator->mBones.size(); ++j)
			{
				Bone* bone = animator->mBones[j];
				glm::mat4 trans = glm::mat4(1.0f);
				if (bone->mIsAnimated[animationName])
				{
					//interpolate for timeframe
					AnimationData& anim = bone->mAnimations[animationName];					
					
					auto posIndex = FindT2inList<glm::vec3>(timeFrame, anim.mKeyPositions);
					auto rotIndex = FindT2inList<glm::quat>(timeFrame, anim.mKeyRotations);
					auto sclIndex = FindT2inList<glm::vec3>(timeFrame, anim.mKeyScalings);

					glm::vec3 position, scale;
					glm::quat rotation;
					glm::mat4 rot = glm::mat4(1.0f);
					if (posIndex.second != -1)
					{
						double T2 = anim.mKeyPositions[posIndex.second].first;
						double T1 = anim.mKeyPositions[posIndex.first].first;
						double localT = (timeFrame - T1) / (T2 - T1);
						position = glm::lerp(anim.mKeyPositions[posIndex.first].second, anim.mKeyPositions[posIndex.second].second, (float)localT);
					}
					else
					{
						position = anim.mKeyPositions[posIndex.first].second;
					}
					if (rotIndex.second != -1)
					{
						double T2 = anim.mKeyRotations[rotIndex.second].first;
						double T1 = anim.mKeyRotations[rotIndex.first].first;
						double localT = (timeFrame - T1) / (T2 - T1);
						rotation = glm::slerp(anim.mKeyRotations[rotIndex.first].second, anim.mKeyRotations[rotIndex.second].second, (float)localT);
						rot = glm::toMat4(rotation);
					}
					else
					{
						rotation = anim.mKeyRotations[rotIndex.first].second;
					}
					if (sclIndex.second != -1)
					{
						double T2 = anim.mKeyScalings[sclIndex.second].first;
						double T1 = anim.mKeyScalings[sclIndex.first].first;
						double localT = (timeFrame - T1) / (T2 - T1);
						scale = glm::lerp(anim.mKeyScalings[sclIndex.first].second, anim.mKeyScalings[sclIndex.second].second, (float)localT);
					}
					else
					{
						scale = anim.mKeyScalings[sclIndex.first].second;
					}
					glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
					glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);
					trans = translate * rot * scaling;
				}
				else
				{
					trans = bone->mTransformation;
				}
				unsigned int parent = bone->mParentIndex;
				if (parent != -1)
				{
					currentTransformations.emplace_back(currentTransformations[parent] * trans);
				}
				else
				{
					currentTransformations.emplace_back(trans);
				}

				animator->mBoneTransformations.push_back(currentTransformations[currentTransformations.size() - 1] * bone->mOffset);
			}
			currentTransformations.clear();
		}
	}

	void AnimationSystem::Update()
	{
		auto res1 = ThreadManager::Instance().Push(THREAD_FUNCTION(AnimationSystem::Animate, 0, mGameObjects.size() / 2));
		auto res2 = ThreadManager::Instance().Push(THREAD_FUNCTION(AnimationSystem::Animate, mGameObjects.size() / 2, mGameObjects.size()));
		res1.get();
		res2.get();
	}
}