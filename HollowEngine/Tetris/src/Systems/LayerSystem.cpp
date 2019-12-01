#include <Hollow.h>
#include "LayerSystem.h"
#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Model.h"
#include "Hollow/Managers/DebugDrawManager.h"
#include "BlockSystem.h"
#include "Hollow/Managers/FrameRateController.h"

LayerSystem LayerSystem::instance;

void LayerSystem::Init()
{
	mMoveInterval = 1.0f;
	mTimePast = 0.0f;
	
	memset(mLayers, false, sizeof(bool) * 1500);
	memset(mFloor, false, sizeof(bool) * 1500);

	mBlockSystem = Hollow::SystemManager::Instance().GetSystem<BlockSystem>();
}

void LayerSystem::Update()
{
	memset(mLayers, false, sizeof(bool) * 1500);
	memcpy(mLayers, mFloor, sizeof(bool) * 1500);

	mTimePast += Hollow::FrameRateController::Instance().GetFrameTime();
	
	if(mTimePast > mMoveInterval)
	{
		mActiveTetrominoPosition.x -= 1;
		mTimePast = 0.0f;
	}
	if (mActiveTetromino)
	{
		mBlockSystem->CopyTetromino(*mActiveTetromino, mLayers, mActiveTetrominoPosition);
	}
	
	for (int i = 0; i < mGameObjects.size() - 1; ++i)
	{
		mGameObjects[i + 1]->mActive = mLayers[i / 100][i % 10][(i / 10) % 10];
		Hollow::Transform* transform = mGameObjects[i + 1]->GetComponent<Hollow::Transform>();
		Hollow::DebugDrawManager::Instance().DebugCube(transform->mPosition, transform->mScale);
	}
}

void LayerSystem::AddGameObject(Hollow::GameObject* object)
{
	CheckAllComponents<Hollow::Transform, Hollow::Model>(object);
}
