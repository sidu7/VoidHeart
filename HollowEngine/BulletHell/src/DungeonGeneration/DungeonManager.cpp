#include "DungeonManager.h"
#include <Hollow.h>
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/ScriptingManager.h"
#include "Hollow/Managers/ImGuiManager.h"

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

		// LUA Bindings for Dungeon Classes
		// Send attack component to lua
		auto& lua = Hollow::ScriptingManager::Instance().lua;

		lua.new_usertype<DungeonFloor>("DungeonFloor",
			sol::constructors<DungeonFloor()>(),
			"GetRoom", &DungeonFloor::GetRoom,
			"GetRoomFromIndex", &DungeonFloor::GetRoomFromIndex,
			"GetRoomCount", &DungeonFloor::GetRoomCount,
			"GetEntrance", &DungeonFloor::GetEntrance,
			"GetEntranceIndex", &DungeonFloor::GetEntranceIndex
			);

		lua.new_usertype<DungeonRoom>("DungeonRoom",
			sol::constructors<DungeonRoom()>(),
			"GetFloorNum", &DungeonRoom::GetFloorNum,
			"GetCoords", &DungeonRoom::GetCoords
			);

		lua.set_function("GetDungeonFloor", &DungeonManager::GetFloor, std::ref(DungeonManager::Instance()));

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
	void DungeonManager::DebugDisplay()
	{
		ImGui::Text("Seed: %u", mSeed);
	}
}
