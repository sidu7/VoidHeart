#include <Hollow.h>
#include "BlockSystem.h"
#include "Hollow/Common.h"

#include "LayerSystem.h"
#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Managers/InputManager.h"

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
	if(Hollow::InputManager::Instance().IsKeyTriggered(SDL_SCANCODE_K))
	{
		Tetromino L = RotateAroundZ(mShapes[0]);
		LayerSystem* layer = Hollow::SystemManager::Instance().GetSystem<LayerSystem>();
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					layer->mLayers[i][j][k] = L.mData[i][j][k];
				}
			}
		}
	}
}

void BlockSystem::AddGameObject(Hollow::GameObject* object)
{
}

Tetromino BlockSystem::RotateAroundZ(Tetromino& data)
{
	bool workingArray[3][3] = { false };

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			bool res = false;
			for(int k = 0; k < 3; ++k)
			{
				res = res || data.mData[2 - i][k][j];
			}
			workingArray[i][j] = res;
		}
	}

	// Mirror working array
	for(int i = 0; i < 3; ++i)
	{
		std::swap(workingArray[0][i], workingArray[2][i]);
		for(int j = 0; j < 3; ++j)
		{
			std::swap(data.mData[2][j][i], data.mData[0][j][i]);
		}
	}

	// Transpose working array
	for(int i = 0; i < 3; ++i)
	{
		for(int j = i; j < 3; ++j)
		{
			if(i==j)
			{
				continue;
			}
			std::swap(workingArray[i][j], workingArray[j][i]);
			for(int k = 0; k < 3; ++k)
			{
				std::swap(data.mData[2 - i][k][j], data.mData[2-j][k][i]);
			}
		}
	}

	return data;
}
