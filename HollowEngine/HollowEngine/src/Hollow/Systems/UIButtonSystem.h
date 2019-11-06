#pragma once
#include "System.h"

namespace Hollow
{
	class HOLLOW_API UIButtonSystem : public System
	{
		REGISTERSYSTEM(UIButtonSystem, 0);
	public:
		virtual ~UIButtonSystem() {}
		void Init();
		void AddGameObject(GameObject* object);
		void Update();

	private:
		bool IsButtonClicked(glm::vec2 mousePosition, glm::vec2 buttonCenter, glm::vec2 buttonScale);
	};
}