#pragma once
#include "Hollow/Systems/System.h"

namespace MarbleBall
{
	class ScoreSystem : public Hollow::System
	{
		REGISTERSYSTEM(ScoreSystem, 3)
	public:
		void Init() override;
		void Update() override;
		void AddGameObject(Hollow::GameObject* pGameObject) override;
		void HandleBroadcastEvent(Hollow::GameEvent& event) override;

	private:
		void OnPointCollected(Hollow::GameEvent& event);
		void OnGoalReached(Hollow::GameEvent& event);
	};
}