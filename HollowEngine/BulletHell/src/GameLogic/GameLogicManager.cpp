#include "GameLogicManager.h"

#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Components/Script.h"
#include "Components/Attack.h"

#include "DungeonGeneration/DungeonRoom.h"

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

    std::string GameLogicManager::GetRandomEnemy()
    {
        return "EnemyFollowsPlayer";
    }

    glm::vec3 GameLogicManager::GetRoomOffset(int, int)
    {
        return glm::vec3(10.0f, 1.5f, 0.0f);
    }

	// TODO: Figure out what an enum means to you
	enum RoomsEnumMaybe
	{

	};

	void GameLogicManager::InitializeRoomsMap()
	{
		// TODO: Add loop over all room files somehow
		// Loads all room files into map
		// for all room files
		std::string path = "Resources/Levels/RoomTest.json";
		std::ifstream file(path);
		std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		mCachedRoomsMap[0] = contents;
	}

	void GameLogicManager::PopulateRoom(DungeonRoom& room)
	{
		// TODO: Somehow randomly or procedurally get room index, maybe in script?
		int roomIndex = 0;
		rapidjson::Document root;
		root.Parse(mCachedRoomsMap[roomIndex].c_str());
		if (!root.IsObject()) { HW_CORE_ERROR("Error reading Room file {0}", roomIndex); }
		rapidjson::Value::Object data = root.GetObject();

		CreateObjectsInRoom(room, data, "Enemies", "EnemyType", true);
		CreateObjectsInRoom(room, data, "Obstacles", "ObstacleType", false);
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
