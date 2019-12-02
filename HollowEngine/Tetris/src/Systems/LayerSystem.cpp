#include <Hollow.h>
#include "LayerSystem.h"
#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Model.h"
#include "Hollow/Managers/DebugDrawManager.h"
#include "BlockSystem.h"
#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Components/UIText.h"

LayerSystem LayerSystem::instance;

void LayerSystem::Init()
{
	mMoveInterval = 1.0f;
	mInterval = mMoveInterval;
	mDropInterval = 0.05f;
	mTimePast = 0.0f;
	
	memset(mLayers, false, sizeof(bool) * 16*12*12);
	memset(mFloor, false, sizeof(bool) * 16*12*12);

	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 12; ++j) {
			for (int k = 0; k < 12; ++k) {
				if(i == 0 || j == 0 || j == 11 || k == 0 || k == 11)
					mFloor[i][j][k] = true;
			}
		}
	}

	mBlockSystem = Hollow::SystemManager::Instance().GetSystem<BlockSystem>();

	mActiveTetrominoPosition = glm::ivec3(0, 0, 0);
	mActiveTetrominoBase = 0;

	mDropping = false;
	mGameOver = false;
}

void LayerSystem::Update()
{
	if (!mGameOver)
	{
		memset(mLayers, false, sizeof(bool) * 16 * 12 * 12);
		memcpy(mLayers, mFloor, sizeof(bool) * 16 * 12 * 12);

		mTimePast += Hollow::FrameRateController::Instance().GetFrameTime();

		if (!mDropping)
		{
			mBlockSystem->Updato();
		}

		if (mTimePast > mInterval)
		{
			CheckForDrop();
			mActiveTetrominoPosition.x -= 1;
			mTimePast = 0.0f;
		}

		if (!mBlockSystem->mSpawnBlock)
		{
			mBlockSystem->CopyTetromino(*mActiveTetromino, mLayers, mActiveTetrominoPosition);
		}
	}
	else
	{
		Hollow::UIText* text = mGameObjects[0]->GetComponent<Hollow::UIText>();
		text->mText = "GAMEOVER!!";
	}
	for (int i = 0; i < mGameObjects.size() - 5; ++i)
	{
		int x = i / 144;
		int y = i % 12;
		int z = (i / 12) % 12;
		if (y != 0 && y != 11 && z != 0 && z != 11 && x != 0)
		{
			mGameObjects[i + 1]->mActive = mLayers[x][y][z];
		}
	}
	for (int i = mGameObjects.size() - 4; i < mGameObjects.size(); ++i)
	{
		Hollow::Transform* transform = mGameObjects[i]->GetComponent<Hollow::Transform>();
		Hollow::DebugDrawManager::Instance().DebugCube(transform->mPosition, transform->mScale);
	}
}

void LayerSystem::AddGameObject(Hollow::GameObject* object)
{
	CheckAllComponents<Hollow::Transform, Hollow::Model>(object);
}

void LayerSystem::CheckForDrop()
{
	if(!mActiveTetromino)
	{
		return;
	}
	int size = mActiveTetromino->size;
	for (int i = 0; i < size; ++i) // height layer
	{
		for (int j = 0; j < size; ++j)
		{
			for (int k = 0; k < size; ++k)
			{
				bool block;
				switch (size)
				{
				case 2:
					block = static_cast<Tetromino2*>(mActiveTetromino)->mData[i][j][k];
					break;
				case 3:
					block = static_cast<Tetromino3*>(mActiveTetromino)->mData[i][j][k];
					break;
				case 4:
					block = static_cast<Tetromino4*>(mActiveTetromino)->mData[i][j][k];
					break;
				}
				bool layerdown = mLayers[mActiveTetrominoPosition.x - 1 + i][j + mActiveTetrominoPosition.y][k + mActiveTetrominoPosition.z];
				if (block && layerdown)
				{
					//cannot move
					MakeFloor();
					return;
				}
			}
		}
	}	
}

void LayerSystem::CheckLines()
{
	int size = mActiveTetromino->size;
	int l = 0;
	for (int i = 0; i < size; ++i) // height layer
	{
		if (CheckLine(l))
		{
			// Delete line l
			for (int m = l + mActiveTetrominoPosition.x; m < 15; ++m)
			{
				for (int j = 0; j < 12; ++j)
				{
					for (int k = 0; k < 12; ++k)
					{
						mFloor[m][j][k] = mFloor[m + 1][j][k];
					}
				}
			}
			for (int j = 0; j < 12; ++j) // Last row false
			{
				for (int k = 0; k < 12; ++k)
				{
					mFloor[15][j][k] = false;
				}
			}
			--l;
		}
		++l;
	}
}

bool LayerSystem::CheckLine(int i)
{
	for (int j = 0; j < 12; ++j)
	{
		for (int k = 0; k < 12; ++k)
		{
			if (!mFloor[mActiveTetrominoPosition.x + i][j][k])
			{
				return false;
			}
		}
	}
	return true;
}

void LayerSystem::MakeFloor()
{
	if(mDropping)
	{
		mDropping = false;
		mInterval = mMoveInterval;
	}
	mBlockSystem->CopyTetromino(*mActiveTetromino, mFloor, mActiveTetrominoPosition);
	CheckLines();	
	mBlockSystem->mSpawnBlock = true;
}
