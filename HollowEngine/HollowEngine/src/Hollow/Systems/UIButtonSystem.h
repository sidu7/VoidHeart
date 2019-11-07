#pragma once
#include "System.h"

namespace Hollow
{
	class UIButtonSystem : public System
	{
		REGISTERSYSTEM(UIButtonSystem, 0);
	public:
		HOLLOW_API virtual ~UIButtonSystem() {}
		HOLLOW_API void Init();
		HOLLOW_API void AddGameObject(GameObject* object);
		HOLLOW_API void Update();

	private:
		bool IsButtonClicked(glm::vec2 mousePosition, glm::vec2 buttonCenter, glm::vec2 buttonScale);
	};
}