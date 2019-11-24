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
		if(CheckAllComponents<Animator,StateMachine>(object))
		{
			Animator* animator = object->GetComponent<Animator>();
			CreateSkeleton(animator);
			StateMachine* stateMachine = object->GetComponent<StateMachine>();
			animator->mCurrentState = stateMachine->mCurrentState->mName;
		}
	}

	void AnimationSystem::Animate(unsigned int start, unsigned int end)
	{
		for (unsigned int i = start; i < end; ++i)
		{
			GameObject* gameobject = mGameObjects[i];
			Animator* animator = gameobject->GetComponent<Animator>();
			StateMachine* state = gameobject->GetComponent<StateMachine>();
			float deltaTime = FrameRateController::Instance().GetFrameTime();
			if (animator->mCurrentState != state->mCurrentState->mName)
			{
				animator->mPreviousState = animator->mCurrentState;
				animator->mCurrentState = state->mCurrentState->mName;
				animator->mLoopingAnimation = state->mCurrentState->mIsLoop;
				animator->mCurrentRunTime = 0.0f;
				if (state->mCurrentState->mIsLoop && state->mPreviousState->mIsLoop)
				{
					animator->mPreviousRunTime = animator->mCurrentRunTime;
					animator->mBlending = true;
					animator->mBlendTime = animator->mBlendFactor;
				}
			}
			
			double prevTimeFrame = 0.0f;
			if(animator->mBlending)
			{
				animator->mPreviousRunTime += deltaTime;
				auto prev_ticks_duration = animator->mAnimations[animator->mPreviousState];
				double PrevTimeinTicks = animator->mPreviousRunTime * prev_ticks_duration.first;
				prevTimeFrame = fmod(PrevTimeinTicks, prev_ticks_duration.second);
				animator->mBlendTime -= deltaTime;
				if(animator->mBlendTime <= 0.0f)
				{
					animator->mBlending = false;
				}
			}
			
			animator->mCurrentRunTime += deltaTime;			
			auto ticks_duration = animator->mAnimations[animator->mCurrentState];
			double TimeinTicks = animator->mCurrentRunTime * ticks_duration.first;
			double animationTime = TimeinTicks / ticks_duration.second;

			// For Non looping animations
			if (animationTime > 1.0 && !animator->mLoopingAnimation)
			{
				state->mNeedChangeState = true;
				break;
			}
			
			double timeFrame = fmod(TimeinTicks, ticks_duration.second);
			animator->mBoneTransformations.clear();
			std::string animationName = animator->mCurrentState;
			std::string prevAnimationName = animator->mPreviousState;
			std::vector<glm::mat4> currentTransformations;
			currentTransformations.reserve(animator->mBones.size());
			auto& skeleton = animator->mSkeleton;
			for (unsigned int j = 0; j < animator->mBones.size(); ++j)
			{
				Bone* bone = animator->mBones[j];
				auto& animData = animator->mAnimationData[j];
				glm::mat4 trans = glm::mat4(1.0f);
				if (animData[animationName].first)
				{
					//interpolate for timeframe
					AnimationData& anim = animData[animationName].second;					
					
					auto posIndex = FindT2inList<glm::vec3>(timeFrame, anim.mKeyPositions);
					auto rotIndex = FindT2inList<glm::quat>(timeFrame, anim.mKeyRotations);
					auto sclIndex = FindT2inList<glm::vec3>(timeFrame, anim.mKeyScalings);

					glm::vec3 position = Interpolate(posIndex, timeFrame, anim.mKeyPositions);
					glm::vec3 scale = Interpolate(sclIndex, timeFrame, anim.mKeyScalings);
					glm::quat rotation = SInterpolate(rotIndex, timeFrame, anim.mKeyRotations);
										
					if (animator->mBlending && animData[prevAnimationName].first)
					{
						AnimationData& prevAnim = animData[prevAnimationName].second;
						auto prevposIndex = FindT2inList<glm::vec3>(prevTimeFrame, prevAnim.mKeyPositions);
						auto prevrotIndex = FindT2inList<glm::quat>(prevTimeFrame, prevAnim.mKeyRotations);
						auto prevsclIndex = FindT2inList<glm::vec3>(prevTimeFrame, prevAnim.mKeyScalings);

						glm::vec3 prevposition = Interpolate(prevposIndex, prevTimeFrame, prevAnim.mKeyPositions);
						glm::vec3 prevscale = Interpolate(prevsclIndex, prevTimeFrame, prevAnim.mKeyScalings);
						glm::quat prevrotation = SInterpolate(prevrotIndex, prevTimeFrame, prevAnim.mKeyRotations);
						
						float factor = animator->mBlendTime / animator->mBlendFactor;
						position = glm::lerp(position, prevposition, factor);
						rotation = glm::slerp(rotation, prevrotation,factor);
						scale = glm::lerp(scale, prevscale, factor);
					}
					
					glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
					glm::mat4 rot = glm::toMat4(rotation);
					glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);
					trans = translate * rot * scaling;
				}
				else
				{
					if(animator->mBlending && animData[prevAnimationName].first)
					{
						AnimationData& prevAnim = animData[prevAnimationName].second;
						auto prevposIndex = FindT2inList<glm::vec3>(prevTimeFrame, prevAnim.mKeyPositions);
						auto prevrotIndex = FindT2inList<glm::quat>(prevTimeFrame, prevAnim.mKeyRotations);
						auto prevsclIndex = FindT2inList<glm::vec3>(prevTimeFrame, prevAnim.mKeyScalings);

						glm::vec3 prevposition = Interpolate(prevposIndex, prevTimeFrame, prevAnim.mKeyPositions);
						glm::vec3 prevscale = Interpolate(prevsclIndex, prevTimeFrame, prevAnim.mKeyScalings);
						glm::quat prevrotation = SInterpolate(prevrotIndex, prevTimeFrame, prevAnim.mKeyRotations);

						float factor = animator->mBlendTime / animator->mBlendFactor;
						glm::vec3 position = GraphicsMath::TranslateFromMatrix(bone->mTransformation);
						auto sclrot = GraphicsMath::ScaleRotationFromMatrix(bone->mTransformation);
						glm::vec3 scale = sclrot.first;
						glm::quat rotation = sclrot.second;
						position = glm::lerp(position, prevposition, factor);
						rotation = glm::slerp(rotation, prevrotation, factor);
						scale = glm::lerp(scale, prevscale, factor);

						glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
						glm::mat4 rot = glm::toMat4(rotation);
						glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);
						trans = translate * rot * scaling;
					}
					else
					{
						trans = bone->mTransformation;						
					}
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

				skeleton[bone->mName] = currentTransformations[currentTransformations.size() - 1];
				animator->mBoneTransformations.push_back(currentTransformations[currentTransformations.size() - 1] * bone->mOffset);
			}
			currentTransformations.clear();
		}
	}

	void AnimationSystem::CreateSkeleton(Animator* animator)
	{
		auto& skeleton = animator->mSkeleton;

		for(unsigned int i = 0; i < animator->mBones.size(); ++i)
		{
			skeleton[animator->mBones[i]->mName] = glm::mat4(1.0f);
		}
	}

	glm::vec3 AnimationSystem::Interpolate(std::pair<unsigned, unsigned>& index, double timeFrame,
		std::vector<std::pair<double, glm::vec3>>& list)
	{
		if (index.second != -1)
		{
			double T2 = list[index.second].first;
			double T1 = list[index.first].first;
			double localT = (timeFrame - T1) / (T2 - T1);
			return glm::lerp(list[index.first].second, list[index.second].second, (float)localT);
		}
		else
		{
			return list[index.first].second;
		}
	}

	glm::quat AnimationSystem::SInterpolate(std::pair<unsigned, unsigned>& index, double timeFrame,
		std::vector<std::pair<double, glm::quat>>& list)
	{
		if (index.second != -1)
		{
			double T2 = list[index.second].first;
			double T1 = list[index.first].first;
			double localT = (timeFrame - T1) / (T2 - T1);
			return glm::slerp(list[index.first].second, list[index.second].second, (float)localT);
		}
		else
		{
			return list[index.first].second;
		}
	}

	void AnimationSystem::Update()
	{
		auto res1 = ThreadManager::Instance().Push(THREAD_FUNCTION(AnimationSystem::Animate, 0, mGameObjects.size() / 2));
		auto res2 = ThreadManager::Instance().Push(THREAD_FUNCTION(AnimationSystem::Animate, mGameObjects.size() / 2, mGameObjects.size()));
		res1.get();
		res2.get();
		//Animate(0, mGameObjects.size());
	}
}