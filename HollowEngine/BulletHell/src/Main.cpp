#include <Hollow.h>
#include "Hollow/Common.h"
#include "Hollow/Core/EntryPoint.h"

#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Managers/SceneManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/EventManager.h"

#include "Components/Health.h"
#include "GameMetaData/GameObjectType.h"
#include "GameMetaData/GameEventType.h"
#include "Hollow/Core/GameMetaData.h"

#include "DungeonGeneration/DungeonManager.h"

#include "Hollow/Components/Body.h"

void Hollow::GameMetaData::Init()
{
	{
#define GAMEOBJECT_TYPE(name) mMapOfGameObjectTypes[#name] = (int)BulletHell::GameObjectType::name;
#include "GameMetaData/GameObjectType.enum"
#undef GAMEOBJECT_TYPE
	}

	{
#define GAME_EVENT(name) mMapOfGameEventTypes[#name] = (int)BulletHell::GameEventType::name;
#include "GameMetaData/GameEvents.enum"
#undef GAME_EVENT
	}
}

class GameLayer : public Hollow::Layer
{
	void OnUpdate()
	{
		// Update Game managers here
	}
};

class BulletHellGame : public Hollow::Application
{
public:
	BulletHellGame()
	{
		Hollow::GameMetaData::Instance().Init();
		// Engine Initialization
		Application::Init("Resources/Settings.json");

		Hollow::SceneManager::Instance().LoadLevel("Level3");
		ConstructRoom();

		PushLayer(new GameLayer());

        BulletHell::DungeonManager::Instance().Init();
	}

	~BulletHellGame()
	{
		HW_TRACE("BulletHell Prototype Closing");
	}

	void ConstructRoom()
	{
		// Programatically create room walls and doors
		// Do this for all rooms, or when desired for a specific room
		// {
	/*	NONE = 0,
			UP = 1,
			RIGHT = 2,
			DOWN = 4,
			LEFT = 8
	*/

	//E.x. room at 3,2 in floor grid with doors at left and top
		int mRoomX = 3;
		int mRoomY = 2;
		int mDoorBits = 15;

		// Origin of room is in top left corner
		const int numRows = 5;
		const int numCols = 5;
		const int halfRow = 2; // floor(numRows/2)
		const int halfCol = 2; // floor(numCols/2)
		for(int rowIndex = 0; rowIndex < numRows; ++rowIndex)
		{
			for(int colIndex = 0; colIndex < numCols; ++colIndex)
			{
				if(colIndex % (numCols-1) == 0 || rowIndex % (numRows -1) == 0)
				{
					// Check door positions
					// UP
					if((colIndex == halfCol) && (rowIndex == 0) && (mDoorBits & 1))
					{
						// Create up door
					}
					else if((colIndex == 0) && (rowIndex == halfRow) && (mDoorBits & 8))
					{
						// Create left door
					}
					else if((rowIndex == halfRow) && (colIndex == numCols-1) && (mDoorBits & 2))
					{
						// Create right door
					}
					else if((rowIndex == numRows-1) && (colIndex == halfRow) && (mDoorBits & 4))
					{
						// Create down door
					}
					else
					{				
						Hollow::GameObject* pRoom = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Prefabs/Wall.json");
						Hollow::Body* pTr = pRoom->GetComponent<Hollow::Body>();
						pTr->mPosition.x += colIndex * 5;
						pTr->mPosition.z += rowIndex * 5;
					}
				}
			}
		}
		
	}
};


Hollow::Application* Hollow::CreateApplication()
{
	return new BulletHellGame();
}