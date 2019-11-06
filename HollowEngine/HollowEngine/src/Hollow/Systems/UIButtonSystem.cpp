#include <hollowpch.h>
#include "UIButtonSystem.h"

#include "Hollow/Components/UITransform.h"
#include "Hollow/Components/UIImage.h"
#include "Hollow/Components/UIButton.h"

#include "Hollow/Managers/InputManager.h"

namespace Hollow
{
	void UIButtonSystem::Init()
	{
	}

	void UIButtonSystem::AddGameObject(GameObject* object)
	{
		CheckAllComponents<UITransform, UIImage, UIButton>(object);
	}

	bool UIButtonSystem::IsButtonClicked(glm::vec2 mousePosition, glm::vec2 buttonCenter, glm::vec2 buttonScale)
	{
		if ((mousePosition.x >= buttonCenter.x - buttonScale.x / 2.0f
			&& mousePosition.x <= buttonCenter.x + buttonScale.x / 2.0f)
			&& (mousePosition.y >= buttonCenter.y - buttonScale.y / 2.0f
				&& mousePosition.y <= buttonCenter.y + buttonScale.y / 2.0f))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void UIButtonSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			UITransform* transform = mGameObjects[i]->GetComponent<UITransform>();
			UIImage* image = mGameObjects[i]->GetComponent<UIImage>();
			UIButton* button = mGameObjects[i]->GetComponent<UIButton>();
			glm::vec2 mousePosition = InputManager::Instance().GetMousePosition();

			if (IsButtonClicked(mousePosition, transform->mPosition, transform->mScale))
			{

			}
			else
			{
				image->mpTexture = button->mpReleasedImage;
			}
		}
	}
}