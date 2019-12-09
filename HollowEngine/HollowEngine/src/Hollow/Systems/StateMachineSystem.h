#pragma once
#include "System.h"

namespace Hollow
{
	/// <summary>
	/// Class StateMachineSystem.
	/// Inherits the <see cref="System" /> class.
	/// </summary>
	/// <seealso cref="System" />
	class StateMachineSystem : public System
	{
		REGISTERSYSTEM(StateMachineSystem, 1);
	public:
		/// <summary>
		/// Destructor for <see cref="StateMachineSystem"/> class.
		/// </summary>
		HOLLOW_API virtual ~StateMachineSystem();
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		HOLLOW_API void Update() override;
		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="object">The object to add.</param>
		HOLLOW_API void AddGameObject(GameObject* object) override;
	};
}