#include <Hollow.h>
#include "ScoreSystem.h"

#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/AudioManager.h"

#include "GameMetaData/GameEventType.h"
#include "GameMetaData/GameObjectType.h"

#include "Components/Score.h"
#include "Hollow/Components/UIText.h"

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
	}

	void ScoreSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Score>(pGameObject);
	}

	void ScoreSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
	{
	}

	void ScoreSystem::OnPointCollected(Hollow::GameEvent& event)
	{
		int sc = 0;
		if(event.mpObject1->mType == (int)GameObjectType::POINT)
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject1);
			Score* score = event.mpObject2->GetComponent<Score>();
			score->mScore += score->mScoreIncrement;
			//Hollow::UIText* text = event.mpObject2->GetComponent<Hollow::UIText>();
			//text->mText = "Score: " + std::to_string(score->mScore);
			sc = score->mScore;
		}
		else
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject2);
			Score* score = event.mpObject1->GetComponent<Score>();
			score->mScore += score->mScoreIncrement;
			sc = score->mScore;
			//Hollow::UIText* text = event.mpObject2->GetComponent<Hollow::UIText>();
			//text->mText = "Score: " + std::to_string(score->mScore);
		}
		Hollow::AudioManager::Instance().PlayEffect("Resources/Audio/SFX/Pickup.wav");
		HW_TRACE("Score {0}", sc);		
	}

	void ScoreSystem::OnGoalReached(Hollow::GameEvent& event)
	{
		if (event.mpObject1->mType == (int)GameObjectType::GOAL)
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject1);
		}
		else
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject2);
		}
		Hollow::AudioManager::Instance().PlayEffect("Resources/Audio/SFX/Goal.wav");
		HW_TRACE("Goal Reached");
	}
}
