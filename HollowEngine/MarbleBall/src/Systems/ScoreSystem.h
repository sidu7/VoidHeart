#pragma once
#include "Hollow/Systems/System.h"

namespace MarbleBall
{
	class ScoreSystem : public Hollow::System
	{
		REGISTERSYSTEM(ScoreSystem, 3)
	public:
		/// <summary>
		/// Initializes the ScoreSystem instance and sets the event it needs to be subscribed to.
		/// </summary>
		void Init() override;

		/// <summary>
		/// Updates the score of the player.
		/// </summary>
		void Update() override;

		/// <summary>
		/// Adds the game object to the ScoreSystem.
		/// </summary>
		/// <param name="pGameObject">The pointer to the game object to be added to the system.</param>
		void AddGameObject(Hollow::GameObject* pGameObject) override;

		/// <summary>
		/// Handles the broadcast event for the ScoreSystem.
		/// </summary>
		/// <param name="event">The event that need to be handled.</param>
		void HandleBroadcastEvent(Hollow::GameEvent& event) override;

	private:
		void OnPointCollected(Hollow::GameEvent& event);
		void OnGoalReached(Hollow::GameEvent& event);
	};
}