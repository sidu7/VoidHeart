#pragma once
#include <Hollow.h>

namespace Hollow
{
	class GameEvent;
	class GameObject;
}

namespace BulletHell
{
	class DungeonRoom;

	class GameLogicManager
    {
    private:
        GameLogicManager();
    public:
        GameLogicManager(const GameLogicManager&) = delete;
        GameLogicManager& operator= (const GameLogicManager&) = delete;
        GameLogicManager(GameLogicManager&&) = delete;
        GameLogicManager& operator=(GameLogicManager&&) = delete;
        static GameLogicManager& Instance();
    public:
        void Init();

        void SubscribeToEvents();
		
		// Load enemies, obstacles, pick ups, and traps into room
		void PopulateRoom(DungeonRoom& room);

		// Loads objects from a room json based on object type string
		void CreateObjectsInRoom(DungeonRoom& room, const rapidjson::Value::Object& data, const std::string& objects, const std::string& type, bool addToEnemylist);

        // Create a PickUp at the centre of the room
        void CreatePickUpInRoom(DungeonRoom& room);

        // Generates Enemy in the room given by roomCoords at the position offset
        Hollow::GameObject* GenerateObjectAtPosition(std::string prefabName, glm::ivec2 roomCoords, glm::vec2 posOffset);

        void OnRoomLockDownDelayed(Hollow::GameEvent& event);
	private: 
		void InitializeRoomsMap();
		void LoadRoomJsons(std::string roomPrefix, int count);
	private:
		std::unordered_map<std::string, std::string> mCachedRoomsMap;
    };
}
