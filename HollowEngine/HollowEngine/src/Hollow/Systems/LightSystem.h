#pragma once
#include "System.h"

namespace Hollow
{
	/// <summary>
	/// Class LightSystem.
	/// Inherits the <see cref="System" /> class.
	/// </summary>
	/// <seealso cref="System" />
	class LightSystem : public System
	{
		/// <summary>
		/// Registers this System with the SystemManager
		/// </summary>
		REGISTERSYSTEM(LightSystem,3)
	public:
		/// <summary>
		/// Destructor for LightSystem
		/// </summary>
		HOLLOW_API virtual ~LightSystem() {}
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		HOLLOW_API void Update();
		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="object">The object to add.</param>
		HOLLOW_API void AddGameObject(GameObject* object);
	};
}