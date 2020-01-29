#include "DungeonManager.h"
#include <Hollow.h>
#include "Hollow/Managers/ResourceManager.h"

namespace BulletHell
{
    void DungeonManager::Init()
    {
        int w = 5;
        int h = 5;
        int numFloors = 1;
        int numRooms = 15;

        BulletHell::DungeonFloor dungeonFloor(w, h, 1);
        for (int i = 0; i < numFloors; i++)
        {
            dungeonFloor.Generate(numRooms);
            dungeonFloor.PrintFloor(9);
            mFloors.push_back(dungeonFloor);
            //dungeonFloor.ResetFloor();
        }
    }
    void DungeonManager::Generate()
    {
        mFloors[0].ConstructFloor();
        Hollow::ResourceManager::Instance().LoadPrefabAtPosition("Player", glm::vec3(15, 5, 0));
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
