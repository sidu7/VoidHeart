#include "DungeonManager.h"

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
        Generate();
    }
    void DungeonManager::Generate()
    {
        const int w = 10;
        const int h = 10;
        const int numFloors = 10;

        for (int i = 0; i < numFloors; i++)
        {
            DungeonFloor dungeonFloor(w, h, 1);
            int numRooms = 10 + 2 * i;
            dungeonFloor.Generate(numRooms, mSeed + i);
            mFloors.push_back(dungeonFloor);
            dungeonFloor.PrintFloor(1);
        }
        //system("PAUSE");
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

    unsigned DungeonManager::GetSeed()
    {
        return mSeed;
    }

    std::vector<DungeonFloor> DungeonManager::GetFloors() const
    {
        return mFloors;
    }

    DungeonFloor DungeonManager::GetFloor(int floorNumber)
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
}