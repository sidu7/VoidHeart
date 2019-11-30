#include <Hollow.h>
#include "BlockSystem.h"
#include "Hollow/Common.h"

#include "LayerSystem.h"
#include "Hollow/Managers/SystemManager.h"

BlockSystem BlockSystem::instance;

void BlockSystem::Init()
{
	// Init tetrominos
	Tetromino L;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				L.mData[i][j][k] = false;
			}
		}
	}
	L.mData[2][1][0] = L.mData[1][1][0] = L.mData[0][1][0] = L.mData[0][1][1] = true;

	mShapes[0] = L;

	mSpawnBlock = true;
}

void BlockSystem::Update()
{
	if(mSpawnBlock)
	{
		LayerSystem* layer = Hollow::SystemManager::Instance().GetSystem<LayerSystem>();
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					layer->mLayers[i][j][k] = mShapes[0].mData[i][j][k];
				}
			}
		}
		mSpawnBlock = false;
	}
}

void BlockSystem::AddGameObject(Hollow::GameObject* object)
{
}
