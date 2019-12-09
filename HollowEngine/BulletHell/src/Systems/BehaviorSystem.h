#pragma once
#include "Hollow/Systems/System.h"
#include "Hollow/Components/Body.h"

namespace BulletHell
{
	class BehaviorSystem : public Hollow::System
	{
		REGISTERSYSTEM(BehaviorSystem, 3)
	public:
		/// <summary>
		/// Initializes the System.
		/// </summary
		void Init() override;

		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		void Update() override;

		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="pGameObject">The gameObject instance to add.</param>
		void AddGameObject(Hollow::GameObject* pGameObject) override;

		/// <summary>
		/// Handles the broadcast event.
		/// </summary>
		/// <param name="event">The event.</param>
		void HandleBroadcastEvent(Hollow::GameEvent& event) override;

	private:
		float mDeltaTime;
		Hollow::Body* mpPlayerBody;
	};
}