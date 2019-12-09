#pragma once
#include "System.h"

namespace Hollow
{
	/// <summary>
	/// Class UIButtonSystem.
	/// Inherits the <see cref="System" /> class.
	/// </summary>
	/// <seealso cref="System" />
	class UIButtonSystem : public System
	{
		REGISTERSYSTEM(UIButtonSystem, 0);
	public:
		/// <summary>
		/// Destructor for <see cref="UIButtonSystem"/> class.
		/// </summary>
		HOLLOW_API virtual ~UIButtonSystem() {}
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

	private:
		bool IsButtonClicked(glm::vec2 mousePosition, glm::vec2 buttonCenter, glm::vec2 buttonScale);
	};
}