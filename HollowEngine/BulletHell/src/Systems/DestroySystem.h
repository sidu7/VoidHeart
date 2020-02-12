#pragma once
#include <Hollow.h>
#include "Hollow/Systems/System.h"

namespace BulletHell
{
	class DestroySystem : public Hollow::System
	{
		REGISTERSYSTEM(DestroySystem, 3);
	public:

		void Init();
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		void Update();

		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="pGameObject">The gameObject instance to add.</param>
		void AddGameObject(Hollow::GameObject* pGameObject);
	};
}