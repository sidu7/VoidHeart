#include <Hollow.h>
#include "ScoreSystem.h"

#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/AudioManager.h"

#include "../GameMetaData/GameEventType.h"
#include "../GameMetaData/GameObjectType.h"

#include "../Components/Score.h"
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
		CheckAllComponents<Score,Hollow::UIText>(pGameObject);
	}

	void ScoreSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
	{
	}

	void ScoreSystem::OnPointCollected(Hollow::GameEvent& event)
	{
		Score* score = nullptr;
		Hollow::UIText* text = nullptr;
		if(event.mpObject1->mType == (int)GameObjectType::POINT)
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject1);
			score = event.mpObject2->GetComponent<Score>();
			text = event.mpObject2->GetComponent<Hollow::UIText>();
		}
		else
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject2);
			score = event.mpObject1->GetComponent<Score>();
			text = event.mpObject1->GetComponent<Hollow::UIText>();
		}
		score->mScore += score->mScoreIncrement;
		std::string ss = "Score: ";
		text->mText = ss + std::to_string(score->mScore);
		Hollow::AudioManager::Instance().PlayEffect("Resources/Audio/SFX/Pickup.wav");
	}

	void ScoreSystem::OnGoalReached(Hollow::GameEvent& event)
	{
		Hollow::UIText* text = nullptr;
		if (event.mpObject1->mType == (int)GameObjectType::GOAL)
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject1);
			text = event.mpObject2->GetComponent<Hollow::UIText>();
		}
		else
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject2);
			text = event.mpObject1->GetComponent<Hollow::UIText>();
		}
		Hollow::AudioManager::Instance().PlayEffect("Resources/Audio/SFX/Goal.wav");
		text->mText = "Goal Reached";
	}
}
