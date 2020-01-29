#include "DungeonManager.h"

namespace BulletHell
{
    void DungeonManager::Init()
    {
        int w = 10;
        int h = 10;
        int numFloors = 1;
        int numRooms = 20;

        BulletHell::DungeonFloor dungeonFloor(w, h, 1);
        for (int i = 0; i < numFloors; i++)
        {
            dungeonFloor.Generate(numRooms);
            dungeonFloor.PrintFloor(1);
            //dungeonFloor.ResetFloor();
        }
    }
    void DungeonManager::Generate()
    {

    }

    void DungeonManager::Regenerate()
    {

    }
    std::vector<DungeonFloor> DungeonManager::GetFloors() const
    {
        return mFloors;
    }

    void DungeonManager::Print() const
    {
        for (const DungeonFloor& floor : mFloors)
        {
            floor.PrintFloor();
        }
    }
}