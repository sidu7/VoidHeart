#include <Hollow.h>
#include "HandSystem.h"

#include "Components/ParentOffset.h"

#include "Hollow/Components/Transform.h"

namespace BulletHell
{
	HandSystem HandSystem::instance;

	void HandSystem::Init()
	{
	}

	void HandSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			// Update position based on offset and parent
			Hollow::Transform* pTr = mGameObjects[i]->GetComponent<Hollow::Transform>();
			ParentOffset* pParentOffset = mGameObjects[i]->GetComponent<ParentOffset>();
			Hollow::Transform* pParentTr = pParentOffset->mpParent->GetComponent<Hollow::Transform>();

			pTr->mPosition = pParentOffset->mOffset + pParentTr->mPosition;
		}
	}

	void HandSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<ParentOffset, Hollow::Transform>(pGameObject);
	}

	void HandSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
	{
	}
}