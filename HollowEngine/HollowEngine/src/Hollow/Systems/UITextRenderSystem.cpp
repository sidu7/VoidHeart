#include <hollowpch.h>
#include "UITextRenderSystem.h"

#include "Hollow/Components/UITransform.h"
#include "Hollow/Components/UIText.h"

#include "Hollow/Managers/RenderManager.h"

namespace Hollow
{
	UITextRenderSystem UITextRenderSystem::instance;
	
	void UITextRenderSystem::Init()
	{
	}

	void UITextRenderSystem::Update()
	{
		for(int i = 0; i < mGameObjects.size(); ++i)
		{
			UITransform* transform = mGameObjects[i]->GetComponent<UITransform>();
			UIText* text = mGameObjects[i]->GetComponent<UIText>();

			UITextData uitext;
			uitext.mText = text->mText;
			uitext.mColor = text->mColor;
			uitext.mPosition = transform->mPosition + text->mOffsetPosition;
			uitext.mScale = transform->mScale * text->mTextScale;

			RenderManager::Instance().mUITextData.emplace_back(uitext);
		}
	}

	void UITextRenderSystem::AddGameObject(GameObject* pGameObject)
	{
		CheckAllComponents<UITransform, UIText>(pGameObject);
	}

	void UITextRenderSystem::HandleBroadcastEvent(GameEvent& event)
	{
	}
}
