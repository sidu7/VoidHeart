#include "GameLogicManager.h"
#include <iostream>
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Components/Script.h"
#include "Components/Attack.h"

#include "DungeonGeneration/DungeonRoom.h"

namespace BulletHell
{
    GameLogicManager::GameLogicManager() { std::cout << "Initialized: GameLogicManager" << std::endl; }

    GameLogicManager& GameLogicManager::Instance()
    {
        static GameLogicManager instance;
        return instance;
    }

    Hollow::GameObject* GameLogicManager::GenerateEnemyAtPosition(std::string prefabName, glm::ivec2 roomCoords, glm::vec2 posOffset)
    {
        Hollow::GameObject* pGo = Hollow::ResourceManager::Instance().LoadPrefabAtPosition("EnemyFollowLookdir",
            glm::vec3(roomCoords.y * DungeonRoom::mRoomSize + DungeonRoom::mRoomSize / 2 + posOffset.x,
                1.5f,
                roomCoords.x * DungeonRoom::mRoomSize + DungeonRoom::mRoomSize / 2 + posOffset.y));

        // TODO adjust enemy level according to dungeon floor

        // Disable any scripts
        Hollow::Script* pS = pGo->GetComponent<Hollow::Script>();
        pS->mIsActive = false;
        Attack* pA = pGo->GetComponent<Attack>();
        pA->mIsActive = false;

        return pGo;
    }

    void GameLogicManager::CreateEnemiesInRoom(DungeonRoom& room)
    {
        glm::ivec2 coords = room.GetCoords();

        int count = Random::RangeSeeded(0, 5);
        for (int i = 0; i < count; ++i)
        {
            std::string enemy = GetRandomEnemy();

            Hollow::GameObject* pGo = GenerateEnemyAtPosition(enemy, coords, GetRoomOffset(count, i));

            room.mEnemies.push_back(pGo);
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
