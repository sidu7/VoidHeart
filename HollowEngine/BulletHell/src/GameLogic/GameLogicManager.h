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

		// Global game object handling
		void InitGlobalGameObjects();
		void ClearNonGlobalGameObjects();
		void AddGlobalGameObject(Hollow::GameObject* pGO);

		void Update();

		void RegisterLuaBindings();
		
        void SubscribeToEvents();
		
		// Load enemies, obstacles, pick ups, and traps into room
		void PopulateRoom(DungeonRoom& room);

		// Loads objects from a room json based on object type string
		void CreateObjectsInRoom(DungeonRoom& room, const rapidjson::Value::Object& data, const std::string& objects, const std::string& type, bool addToEnemylist);

		// Event handling
		void OnPlayerDeath(Hollow::GameEvent& event);
		void OnPlayerHitPortal(Hollow::GameEvent& event);
		void OnDeath(Hollow::GameEvent& event);
		void DropRandomPickup(Hollow::GameObject* pGO);
		void OnRoomLockDownDelayed(Hollow::GameEvent& event);
		void OnSpellCollect(Hollow::GameEvent& event);
		void OnFloorCleared(Hollow::GameEvent& event);

        // Create a PickUp at the centre of the room
        void CreatePickUpInRoom(DungeonRoom& room);
		void OnPickupCollected(Hollow::GameEvent& event);
		void OnPickupEffectEnd(Hollow::GameEvent& event);

		// Gameplay Methods
		void CreateMainMenu();
		void StartNewGame();
        void MoveToNextFloor();
		std::vector<int> GetSpellOrder() { return mSpellOrder; }

		// Main Menu Methods
		void ConstructMainMenuRoom();
		void CreateRoomLabels();
		void CreateOptionsUI();
		void CreateCreditsUI();
		void CreateExitConfirmationUI();
		
        // Generates Enemy in the room given by roomCoords at the position offset
        Hollow::GameObject* GenerateObjectAtPosition(std::string prefabName, glm::ivec2 roomCoords, glm::vec2 posOffset);
		
	private: 
		// Cheat codes
		void CheckCheatCodes();
		void UpdateSplashScreen();
		void CheckKillPlane();
		void InitializeRoomsMap();
		void AddBuffs(Hollow::GameObject* pGo, Pickup* pPickup);
		void LoadRoomJsons(std::string roomPrefix, int count);
		void OnBulletHitShield(Hollow::GameEvent& event);
		void FireToggleFullScreenEvent();
		void CheckIfPlayerInBossRoom();
	private:
		bool isFullScreen;
		ImGuiWindowFlags mWindowFlags;
		std::vector<Hollow::GameObject*> mGlobalGameObjects;
		Hollow::GameObject* mpSplashScreen;
		Hollow::GameObject* mCreditsUIObject;
		Hollow::GameObject* mpPlayerGO;
		std::unordered_map<std::string, std::string> mCachedRoomsMap;
		std::vector<std::string> mPickupPrefabNames;
		int mCountDeadEnemies;
		int mRandomCount;
		std::vector<int> mSpellOrder;
		bool mIsChangingFloors;
		float mPreviousPlayerSpeed;

		float splashTime;
    };
}
