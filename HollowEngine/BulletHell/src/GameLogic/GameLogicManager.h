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
	class Pickup;
	class GameEvent;

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

		static bool hasGameStarted;
    public:
        void Init();

		void StartNewGame();

		void RegisterLuaBindings();
		
        void SubscribeToEvents();
		
		// Load enemies, obstacles, pick ups, and traps into room
		void PopulateRoom(DungeonRoom& room);

		// Loads objects from a room json based on object type string
		void CreateObjectsInRoom(DungeonRoom& room, const rapidjson::Value::Object& data, const std::string& objects, const std::string& type, bool addToEnemylist);

		void DropRandomPickup(Hollow::GameEvent& event);

        // Create a PickUp at the centre of the room
        void CreatePickUpInRoom(DungeonRoom& room);
		void OnPickupCollected(Hollow::GameEvent& event);
		void OnPickupEffectEnd(Hollow::GameEvent& event);

		void ConstructMainMenuRoom();

        void MoveToNextFloor();

		void CreateRoomLabels();
		
        // Generates Enemy in the room given by roomCoords at the position offset
        Hollow::GameObject* GenerateObjectAtPosition(std::string prefabName, glm::ivec2 roomCoords, glm::vec2 posOffset);

        void OnRoomLockDownDelayed(Hollow::GameEvent& event);
	private: 
		void InitializeRoomsMap();
		void AddBuffs(Hollow::GameObject* pGo, Pickup* pPickup);
		void LoadRoomJsons(std::string roomPrefix, int count);

		void OnBulletHitShield(Hollow::GameEvent& event);

	private:
		std::unordered_map<std::string, std::string> mCachedRoomsMap;
		std::vector<std::string> mPickupPrefabNames;
		int mCountDeadEnemies;
		int mRandomCount;
    };
}
