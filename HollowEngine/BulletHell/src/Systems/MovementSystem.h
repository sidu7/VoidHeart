#pragma once
#include "Hollow/Systems/System.h"

namespace BulletHell
{
	class MovementSystem : public Hollow::System
	{
		REGISTERSYSTEM(MovementSystem, 2);
	public:
		void Init() override;
		
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		void Update();

		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="pGameObject">The gameObject instance to add.</param>
		void AddGameObject(Hollow::GameObject* pGameObject);

	private:
		void OnPlayerAOESlow(Hollow::GameEvent& event);
	};
}