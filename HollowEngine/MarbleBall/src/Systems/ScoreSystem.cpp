#include <Hollow.h>
#include "ScoreSystem.h"

#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/AudioManager.h"

#include "../GameMetaData/GameEventType.h"
#include "../GameMetaData/GameObjectType.h"

#include "../Components/Score.h"
#include "Hollow/Components/UIText.h"
#include "Hollow/Managers/LocalizationManager.h"

namespace MarbleBall
{
	ScoreSystem ScoreSystem::instance;
	
	void ScoreSystem::Init()
	{
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_POINT_COLLECTED, EVENT_CALLBACK(ScoreSystem::OnPointCollected));
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_GOAL_COMPLETE, EVENT_CALLBACK(ScoreSystem::OnGoalReached));
	}

	void ScoreSystem::Update()
	{
		for(int i = 0; i < mGameObjects.size(); ++i)
		{
			Score* score = mGameObjects[i]->GetComponent<Score>();
			Hollow::UIText* text = mGameObjects[i]->GetComponent<Hollow::UIText>();
			if (!score->mGoalReached)
			{
				std::string ss = Hollow::LocalizationManager::Instance().mCurrentLanguageMap["SCORE"];
				text->mText = ss + std::to_string(score->mScore);
			}
			else
			{
				text->mText = Hollow::LocalizationManager::Instance().mCurrentLanguageMap["GOAL"];
			}
		}
	}

	void ScoreSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Score,Hollow::UIText>(pGameObject);
	}

	void ScoreSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
	{
	}

	void ScoreSystem::OnPointCollected(Hollow::GameEvent& event)
	{
		Score* score = nullptr;		
		if(event.mpObject1->mType == (int)GameObjectType::POINT)
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject1);
			score = event.mpObject2->GetComponent<Score>();
		}
		else
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject2);
			score = event.mpObject1->GetComponent<Score>();
		}
		score->mScore += score->mScoreIncrement;		
		Hollow::AudioManager::Instance().PlayEffect("Resources/Audio/SFX/Pickup.wav");
	}

	void ScoreSystem::OnGoalReached(Hollow::GameEvent& event)
	{
		Score* score = nullptr;
		if (event.mpObject1->mType == (int)GameObjectType::GOAL)
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject1);
			score = event.mpObject2->GetComponent<Score>();
		}
		else
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject2);
			score = event.mpObject1->GetComponent<Score>();
		}
		
		Hollow::AudioManager::Instance().PlayEffect("Resources/Audio/SFX/Goal.wav");
		score->mGoalReached = true;
	}
}
