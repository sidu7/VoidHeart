#pragma once
#include "Hollow/Systems/System.h"

namespace Hollow
{
	class GameObject;
}

namespace BulletHell
{

	class HealthSystem : public Hollow::System
	{
		REGISTERSYSTEM(HealthSystem, 3)
	public:
		/// <summary>
		/// Initializes the System.
		/// </summary>
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
		void OnBulletHitPlayer(Hollow::GameEvent& event);
		void OnBulletHitWall(Hollow::GameEvent& event);
		void OnBulletHitDoor(Hollow::GameEvent& event);
		void OnPlayerBulletHitEnemy(Hollow::GameEvent& event);
		void OnAOEDamageHitPlayer(Hollow::GameEvent& event);

		void HandleBulletDamage(Hollow::GameObject* pObjectHit, Hollow::GameObject* pBullet);

		void CreateHPUIIcon(int index);

	private:
		std::vector<Hollow::GameObject*> mPlayerHPUIIcons;
	};
}