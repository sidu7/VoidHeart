#pragma once

//#include "Hollow/Common.h"
#include "DungeonFloor.h"
#include "Hollow/Utils/Singleton.h"
#include <Hollow.h>
#include <Hollow/Core/GameObject.h>


namespace Hollow {
	class GameEvent;
}

namespace BulletHell
{
    class DungeonManager
    {
    private: 
        DungeonManager();
    public: 
        DungeonManager(const DungeonManager&) = delete;
        DungeonManager& operator= (const DungeonManager&) = delete;
        DungeonManager(DungeonManager&&) = delete;
        DungeonManager& operator=(DungeonManager&&) = delete;
        static DungeonManager& Instance();
    public:
        void Init();
        void Generate();
        void Regenerate();
        void Construct();
        void ConfigureDungeon();
        bool SetSeed(unsigned seed);
        unsigned GetSeed();

        std::vector<DungeonFloor> GetFloors() const;
        DungeonFloor& GetFloor(int index);
        void Print() const;

    	// Create Enemies adjusted according to the floor the room is in
        void CreateEnemiesInRoom(DungeonRoom& room);

    	// Create a PickUp at the centre of the room
        void CreatePickUpInRoom(DungeonRoom& room);
    	
		void DebugDisplay();
    private:
        int length;
        int breadth;
        int numFloors;
        int firstFloorRoomCount;

        void SubscribeToEvents();
        void OnDeath(Hollow::GameEvent& event);

        DungeonRoom& GetCurrentRoom();
        void OnFloorCleared(Hollow::GameEvent& event);
    public:
		Hollow::GameObject* mpPlayerGo;
    private:
        std::vector<DungeonFloor> mFloors;
        unsigned mSeed;
    };
}
