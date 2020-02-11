#pragma once
#include <glm/vec2.hpp>
#include <string>
#include "Hollow/Core/GameObject.h"

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

        // Create Enemies adjusted according to the floor the room is in
        void CreateEnemiesInRoom(DungeonRoom& room);

        // Create a PickUp at the centre of the room
        void CreatePickUpInRoom(DungeonRoom& room);

        // Generates Enemy in the room given by roomCoords at the position offset
        Hollow::GameObject* GenerateEnemyAtPosition(std::string prefabName, glm::ivec2 roomCoords, glm::vec2 posOffset);

        std::string GetRandomEnemy();

        glm::vec3 GetRoomOffset(int, int);
    };
}
