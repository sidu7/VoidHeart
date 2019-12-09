#pragma once

#include "System.h"

namespace Hollow
{
	/// <summary>
	/// Class RenderSystem.
	/// Inherits the <see cref="System" /> class.
	/// </summary>
	/// <seealso cref="System" />
	class RenderSystem : public System
	{
		REGISTERSYSTEM(RenderSystem,4)
	public:
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