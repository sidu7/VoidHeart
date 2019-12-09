#pragma once
#include "System.h"

namespace Hollow
{
	/// <summary>
	/// Class UIRenderSystem.
	/// Inherits the <see cref="System" /> class.
	/// </summary>
	/// <seealso cref="System" />
	class UIRenderSystem : public System
	{
		REGISTERSYSTEM(UIRenderSystem, 4);
	public:
		/// <summary>
		/// Destructor for <see cref="UIRenderSystem"/> class.
		/// </summary>
		HOLLOW_API virtual ~UIRenderSystem() {}
		/// <summary>
		/// Initializes the System.
		/// </summary>
		HOLLOW_API void Init() override;
		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="object">The object to add.</param>
		HOLLOW_API void AddGameObject(GameObject* object) override;
		/// <summary>
		/// Updates the System every frame.
		/// </summary>
		HOLLOW_API void Update() override;
	};
}
