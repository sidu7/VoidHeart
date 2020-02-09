#include "DungeonManager.h"

#include "Hollow/Components/Transform.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/ScriptingManager.h"
#include "Hollow/Managers/ImGuiManager.h"
#include "Hollow/Managers/EventManager.h"

#include "GameMetaData/GameEventType.h"
#include "Events/DeathEvent.h"

namespace BulletHell
{
    DungeonManager::DungeonManager() : mSeed(0) { std::cout << "Initialized: DungeonManager" << std::endl; }

    DungeonManager& DungeonManager::Instance()
    {
        static DungeonManager instance;
        return instance;
    }
    void DungeonManager::Init()
    {
        //mSeed = 521288629;
        auto randomizer = Random::Range(0, MAXINT);
        mSeed = randomizer();
        Generate();

        SubscribeToEvents();
    	
		// LUA Bindings for Dungeon Classes
		// Send attack component to lua
		auto& lua = Hollow::ScriptingManager::Instance().lua;

		lua.new_usertype<DungeonFloor>("DungeonFloor",
			sol::constructors<DungeonFloor()>(),
			"GetRoom", &DungeonFloor::GetRoom,
			"GetRoomFromIndex", &DungeonFloor::GetRoomFromIndex,
			"GetRegularRoom", &DungeonFloor::GetRegularRoom,
			"GetRoomCount", &DungeonFloor::GetRoomCount,
			"GetEntrance", &DungeonFloor::GetEntrance,
			"GetEntranceIndex", &DungeonFloor::GetEntranceIndex
			);

		lua.new_usertype<DungeonRoom>("DungeonRoom",
			sol::constructors<DungeonRoom()>(),
			"GetFloorNum", &DungeonRoom::GetFloorNum,
			"GetDoorBits", &DungeonRoom::GetDoorBits,
			"GetCoords", &DungeonRoom::GetCoords,
			"IsCleared", &DungeonRoom::IsCleared,
            "UnlockRoom", &DungeonRoom::UnlockRoom,
            "LockDownRoom", &DungeonRoom::LockDownRoom,
            "getEnemyCount", &DungeonRoom::GetEnemyCount
			);

		lua.set_function("GetDungeonFloor", &DungeonManager::GetFloor, std::ref(DungeonManager::Instance()));
		lua.set_function("CreateEnemiesInRoom", &DungeonManager::CreateEnemiesInRoom, std::ref(DungeonManager::Instance()));
		lua.set_function("CreatePickUpInRoom", &DungeonManager::CreatePickUpInRoom, std::ref(DungeonManager::Instance()));

		// Add to ImGui display
		Hollow::ImGuiManager::Instance().AddDisplayFunction("Dungeon", std::bind(&DungeonManager::DebugDisplay, &DungeonManager::Instance()));
    }

    void DungeonManager::Generate()
    {

        for (int i = 0; i < numFloors; i++)
        {
            DungeonFloor dungeonFloor(length, breadth, 1);
            int numRooms = firstFloorRoomCount + 2 * i;
            dungeonFloor.Generate(numRooms, mSeed + i);
            mFloors.push_back(dungeonFloor);
            dungeonFloor.PrintFloor(1);
        }
        //system("PAUSE");
        Construct();
    }

    void DungeonManager::Regenerate()
    {
        // delete old data
        // ...
        for (DungeonFloor& dungeonFloor : mFloors)
        {
            dungeonFloor.ResetFloor();
        }
        Generate();
    }

    void DungeonManager::SetSeed(unsigned seed)
    {
        mSeed = seed;
    }
	
    void DungeonManager::Construct()
    {
        mFloors[0].ConstructFloor();
    }

    void DungeonManager::ConfigureDungeon()
    {
    	// Set the configuration values read from GameConfig.lua into DungeonManager class
        auto& luaState = Hollow::ScriptingManager::Instance().lua;
        firstFloorRoomCount = luaState["firstFloorRoomCount"];
        numFloors = luaState["numFloors"];
        length = luaState["dungeonLength"];
        breadth = luaState["dungeonBreadth"];

        DungeonRoom::mWallLength = luaState["wallLength"];
        DungeonRoom::mWallHeight = luaState["wallHeight"];
        DungeonRoom::mWallThickness = luaState["wallThickness"];
        DungeonRoom::mRoomSize = luaState["roomSize"];
        DungeonRoom::mDoorWidth = luaState["doorWidth"];
        DungeonRoom::mDoorHeight = luaState["doorHeight"];
        DungeonRoom::mDoorThickness = luaState["doorThickness"];
    }

    unsigned DungeonManager::GetSeed()
    {
        return mSeed;
    }

    std::vector<DungeonFloor> DungeonManager::GetFloors() const
    {
        return mFloors;
    }

    DungeonFloor& DungeonManager::GetFloor(int floorNumber)
    {
        return mFloors[floorNumber];
    }

    void DungeonManager::Print() const
    {
        for (const DungeonFloor& floor : mFloors)
        {
            floor.PrintFloor();
        }
    }

    void DungeonManager::CreateEnemiesInRoom(DungeonRoom& room)
    {
        glm::ivec2 coords = room.GetCoords();
		
        room.mEnemies.push_back(Hollow::ResourceManager::Instance().LoadPrefabAtPosition("Enemy",
            glm::vec3(coords.y * DungeonRoom::mRoomSize + DungeonRoom::mRoomSize / 2 + 10,
                1.5f,
                coords.x * DungeonRoom::mRoomSize + DungeonRoom::mRoomSize / 2)));

    	room.mEnemies.push_back(Hollow::ResourceManager::Instance().LoadPrefabAtPosition("Enemy",
        glm::vec3(coords.y * DungeonRoom::mRoomSize + DungeonRoom::mRoomSize / 2 - 10,
            1.5f,
            coords.x * DungeonRoom::mRoomSize + DungeonRoom::mRoomSize / 2)));
        
    }

    void DungeonManager::CreatePickUpInRoom(DungeonRoom& room)
    {
        glm::ivec2 coords = room.GetCoords();
        Hollow::ResourceManager::Instance().LoadPrefabAtPosition("AirSpell",
            glm::vec3(coords.y * DungeonRoom::mRoomSize + DungeonRoom::mRoomSize / 2,
                1.5,
                coords.x * DungeonRoom::mRoomSize + DungeonRoom::mRoomSize / 2));
    }

    void DungeonManager::DebugDisplay()
	{
		ImGui::Text("Seed: %u", mSeed);
	}

    void DungeonManager::OnDeath(Hollow::GameEvent& event)
    {
        DeathEvent& pDeathEvent = dynamic_cast<DeathEvent&>(event);
        DungeonRoom& room = GetCurrentRoom();
    	
    	if(pDeathEvent.mType == (int)GameObjectType::ENEMY)
    	{
            auto iter = std::find(room.mEnemies.begin(), room.mEnemies.end(), pDeathEvent.mpObject1);
    		if(iter != room.mEnemies.end())
    		{
                room.mEnemies.erase(iter);
    		}
    	}
    }

    DungeonRoom& DungeonManager::GetCurrentRoom()
    {
        auto& lua = Hollow::ScriptingManager::Instance().lua;
        int currentFloor = lua["currentFloor"].get<int>();
        int currentRoom = lua["currentRoom"].get<int>();
    	
        return mFloors[currentFloor].GetRoomFromIndex(currentRoom);
    }

    void DungeonManager::SubscribeToEvents()
	{
        Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::DEATH, EVENT_CALLBACK(DungeonManager::OnDeath));
	}

}
