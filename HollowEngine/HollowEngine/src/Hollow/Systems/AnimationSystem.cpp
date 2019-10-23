#include <hollowpch.h>
#include "AnimationSystem.h"

#include "Hollow/Components/Animator.h"
#include "Hollow/Components/StateMachine.h"

#include "Hollow/Graphics/Data/Bone.h"
#include "Hollow/Core/Data/StateData.h"

#include "Hollow/Managers/FrameRateController.h"

namespace Hollow
{
	AnimationSystem AnimationSystem::instance;
	
	void AnimationSystem::AddGameObject(GameObject* object)
	{
		CheckComponents<Animator,StateMachine>(object);
	}

	void AnimationSystem::Update()
	{
		mRunTime += FrameRateController::Instance().GetFrameTime();

		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			GameObject* gameobject = mGameObjects[i];
			Animator* animator = gameobject->GetComponent<Animator>();
			StateMachine* state = gameobject->GetComponent<StateMachine>();
			animator->mBoneTransformations.clear();
			std::string animationName = state->mCurrentState->mName;
			for (unsigned int j = 0; j < animator->mBones.size(); ++j)
			{
				Bone* bone = animator->mBones[j];
				glm::mat4 trans = glm::mat4(1.0f);
				if (bone->mIsAnimated[animationName])
				{
					//interpolate for timeframe
					AnimationData& anim = bone->mAnimations[animationName];
					double TimeinTicks = mRunTime * anim.mTicksPerSec;
					double timeFrame = fmod(TimeinTicks, anim.mDuration);
					unsigned int posIndex = FindT2inList<glm::vec3>(timeFrame, anim.mKeyPositions);
					unsigned int rotIndex = FindT2inList<glm::quat>(timeFrame, anim.mKeyRotations);
					unsigned int sclIndex = FindT2inList<glm::vec3>(timeFrame, anim.mKeyScalings);

					glm::vec3 position, scale;
					glm::quat rotation;
					glm::mat4 rot = glm::mat4(1.0f);
					if (posIndex != 0)
					{
						double T2 = anim.mKeyPositions[posIndex].first;
						double T1 = anim.mKeyPositions[posIndex - 1].first;
						double localT = (timeFrame - T1) / (T2 - T1);
						position = glm::lerp(anim.mKeyPositions[posIndex - 1].second, anim.mKeyPositions[posIndex].second, (float)localT);
					}
					else
					{
						position = anim.mKeyPositions[0].second;
					}
					if (rotIndex != 0)
					{
						double T2 = anim.mKeyRotations[rotIndex].first;
						double T1 = anim.mKeyRotations[rotIndex - 1].first;
						double localT = (timeFrame - T1) / (T2 - T1);
						rotation = glm::slerp(anim.mKeyRotations[rotIndex - 1].second, anim.mKeyRotations[rotIndex].second,(float)localT);
						rot = glm::toMat4(rotation);
					}
					else
					{
						rotation = anim.mKeyRotations[0].second;
					}
					if (sclIndex != 0)
					{
						double T2 = anim.mKeyScalings[sclIndex].first;
						double T1 = anim.mKeyScalings[sclIndex - 1].first;
						double localT = (timeFrame - T1) / (T2 - T1);
						scale = glm::lerp(anim.mKeyScalings[sclIndex - 1].second, anim.mKeyScalings[sclIndex].second, (float)localT);
					}
					else
					{
						scale = anim.mKeyScalings[0].second;
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
					bone->mCurrentTransformation = animator->mBones[parent]->mCurrentTransformation * trans;
				}
				else
				{
					bone->mCurrentTransformation = trans;
				}

				animator->mBoneTransformations.push_back(bone->mCurrentTransformation * bone->mOffset);
			}
		}
	}
}