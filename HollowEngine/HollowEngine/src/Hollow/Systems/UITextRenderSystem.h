#pragma once
#include "System.h"

namespace Hollow
{
	/// <summary>
	/// Class UITextRenderSystem.
	/// Inherits the <see cref="System" /> class.
	/// </summary>
	/// <seealso cref="System" />
	class UITextRenderSystem : public System
	{
		REGISTERSYSTEM(UITextRenderSystem, 4)
	public:
		/// <summary>
		/// Initializes the System.
		/// </summary>
		HOLLOW_API void Init() override;
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		HOLLOW_API void Update() override;
		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="pGameObject">The gameObject instance to add.</param>
		HOLLOW_API void AddGameObject(GameObject* pGameObject) override;
		/// <summary>
		/// Handles the broadcast event.
		/// </summary>
		/// <param name="event">The event.</param>
		HOLLOW_API void HandleBroadcastEvent(GameEvent& event) override;
	private:
	};
}