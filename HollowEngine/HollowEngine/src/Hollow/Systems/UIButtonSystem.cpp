#include <hollowpch.h>
#include "UIButtonSystem.h"

#include "Hollow/Components/UITransform.h"
#include "Hollow/Components/UIImage.h"
#include "Hollow/Components/UIButton.h"

#include "Hollow/Managers/InputManager.h"
#include "Hollow/Managers/RenderManager.h"

namespace Hollow
{
	UIButtonSystem UIButtonSystem::instance;

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
			UIButton* button = mGameObjects[i]->GetComponent<UIButton>();
			UIImage* image = mGameObjects[i]->GetComponent<UIImage>();
			
			if (!button->mIsInteractible)
			{
				image->mpTexture = button->mpInactiveImage;
				image->mColor = button->mInactiveColor;
				continue;
			}
			UITransform* transform = mGameObjects[i]->GetComponent<UITransform>();
			glm::vec2 mousePosition = InputManager::Instance().GetMousePosition();
			mousePosition.y = RenderManager::Instance().GetWindowSize().y - mousePosition.y;
			if (IsButtonClicked(mousePosition, transform->mPosition, transform->mScale))
			{
				image->mpTexture = button->mpReleasedImage;
				image->mColor = button->mPressedColor;
				for (auto function : button->mFunctions)
				{
					function();
				}
			}
			else
			{
				image->mpTexture = button->mpReleasedImage;
				image->mColor = button->mReleasedColor;
			}
		}
	}
}