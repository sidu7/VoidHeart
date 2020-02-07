#pragma once

#include "Hollow/Core/Core.h"
#include "Hollow/Systems/System.h"

namespace Hollow {
	/// <summary>
	/// Class ScriptingSystem.
	/// Inherits the <see cref="System" /> class.
	/// </summary>
	/// <seealso cref="System" />
	class ScriptingSystem :public System
	{
		REGISTERSYSTEM(ScriptingSystem, 2)
	public:
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		HOLLOW_API void Update() override;
		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="pGameObject">The gameObject instance to add.</param>
		HOLLOW_API void AddGameObject(GameObject* pGameObject) override;

		void OnDeleteGameObject(GameObject* pGameObject) override;
	};
}
