#include "GameLogicManager.h"

#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Components/Script.h"
#include "Components/Attack.h"

#include "DungeonGeneration/DungeonRoom.h"
#include "DungeonGeneration/DungeonManager.h"
#include "Hollow/Managers/EventManager.h"
#include "GameMetaData/GameEventType.h"

#define MAX_REGULAR_ROOMS 8
#define MAX_BOSS_ROOMS 2

namespace BulletHell
{
    GameLogicManager::GameLogicManager() 
	{
		// Preload all rooms into map
		InitializeRoomsMap();
		std::cout << "Initialized: GameLogicManager" << std::endl; 
	}

    GameLogicManager& GameLogicManager::Instance()
    {
        static GameLogicManager instance;
        return instance;
    }

    void GameLogicManager::Init()
    {
		SubscribeToEvents();
    }

    void GameLogicManager::SubscribeToEvents()
    {
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ROOM_LOCKDOWN_DELAYED, EVENT_CALLBACK(GameLogicManager::OnRoomLockDownDelayed));
    }

    Hollow::GameObject* GameLogicManager::GenerateObjectAtPosition(std::string prefabName, glm::ivec2 roomCoords, glm::vec2 posOffset)
    {
		Hollow::GameObject* pGo = Hollow::ResourceManager::Instance().LoadPrefabAtPosition(prefabName,
			glm::vec3(roomCoords.y * DungeonRoom::mRoomSize + posOffset.x,
				1.5f,
				roomCoords.x * DungeonRoom::mRoomSize + posOffset.y));

        // TODO adjust enemy level according to dungeon floor

        // Disable any scripts
        Hollow::Script* pS = pGo->GetComponent<Hollow::Script>();
		if (pS != nullptr)
		{
			pS->mIsActive = false;
		}
        Attack* pA = pGo->GetComponent<Attack>();
		if (pA != nullptr)
		{
			pA->mIsActive = false;
		}

        return pGo;
    }

    void GameLogicManager::OnRoomLockDownDelayed(Hollow::GameEvent& event)
    {
		auto room = DungeonManager::Instance().GetCurrentRoom();
		for (auto enemy : room.mEnemies)
		{
			// Enable any scripts
			Hollow::Script* pS = enemy->GetComponent<Hollow::Script>();
			if (pS != nullptr)
			{
				pS->mIsActive = true;
			}
			Attack* pA = enemy->GetComponent<Attack>();
			if (pA != nullptr)
			{
				pA->mIsActive = true;
			}
		}
    }

    void GameLogicManager::InitializeRoomsMap()
	{
		// Loads all room files into map
		// for all room files
		LoadRoomJsons("Room", MAX_REGULAR_ROOMS);

		// Do the same for boss rooms
		LoadRoomJsons("Boss/Room", MAX_BOSS_ROOMS);
	}

	void GameLogicManager::LoadRoomJsons(std::string roomPrefix, int count)
	{
		std::string path = "Resources/Json data/Rooms/";
		for (int i = 1; i <= count; ++i)
		{
			std::string name = roomPrefix + std::to_string(i);
			std::ifstream file(path + name + ".json");
			std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();
			mCachedRoomsMap[name] = contents;
		}
	}

	void GameLogicManager::PopulateRoom(DungeonRoom& room)
	{
		int roomNum = 0; 
		std::string roomName;
		if(room.mRoomType == DungeonRoomType::BOSS)
		{
			roomNum = Random::RangeSeeded(1, MAX_BOSS_ROOMS);
			roomName = "Boss/Room" + std::to_string(roomNum);
		}
		else
		{
			roomNum = Random::RangeSeeded(1, MAX_REGULAR_ROOMS);
			roomName = "Room" + std::to_string(roomNum);
		}
		
		rapidjson::Document root;
		root.Parse(mCachedRoomsMap[roomName].c_str());
		if (!root.IsObject()) { HW_CORE_ERROR("Error reading Room file {0}", roomName); }
		rapidjson::Value::Object data = root.GetObject();

		if(data.HasMember("Enemies"))
		{
			CreateObjectsInRoom(room, data, "Enemies", "EnemyType", true);
		}

		if(data.HasMember("Obstacles"))
		{
			CreateObjectsInRoom(room, data, "Obstacles", "ObstacleType", false);
		}
		
	}

	void GameLogicManager::CreateObjectsInRoom(DungeonRoom& room, const rapidjson::Value::Object& data, const std::string& objects, const std::string& type, bool addToEnemylist)
	{
		glm::ivec2 coords = room.GetCoords();
		// Parse all objects in file
		std::string prefabName = "";
		glm::vec2 position(0.0f, 0.0f);
		if (data[objects.c_str()].IsArray())
		{
			auto objectArray = data[objects.c_str()].GetArray();
			for (int objectIndex = 0; objectIndex < objectArray.Size(); ++objectIndex)
			{
				auto object = objectArray[objectIndex].GetObject();
				// Get object type, this is the json prefab file that will be loaded
				if (object[type.c_str()].IsString())
				{
					prefabName = object[type.c_str()].GetString();
				}
				// Get object position
				if (object["Position"].IsArray())
				{
					auto pos = object["Position"].GetArray();
					position = glm::vec2(pos[0].GetFloat(), pos[1].GetFloat());
				}

				// Load enemy prefab at position relative to room
				Hollow::GameObject* pGo = GenerateObjectAtPosition(prefabName, coords, position);
				if (addToEnemylist)
				{
					room.mEnemies.push_back(pGo);
				}
			}
		}
	}

    void GameLogicManager::CreatePickUpInRoom(DungeonRoom& room)
    {
        glm::ivec2 coords = room.GetCoords();
        Hollow::ResourceManager::Instance().LoadPrefabAtPosition("AirSpell",
            glm::vec3(coords.y * DungeonRoom::mRoomSize + DungeonRoom::mRoomSize / 2,
                1.5,
                coords.x * DungeonRoom::mRoomSize + DungeonRoom::mRoomSize / 2));
    }
}
