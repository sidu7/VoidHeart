-- Setups a Floor of the Dungeon 
local floor = GetDungeonFloor(currentFloor)

-- update current room to the dungeon entrance
currentRoom = floor:GetEntranceIndex()
local coords = floor:GetEntrance():GetCoords()
--CreatePrefabAtPosition("EnemyBoss1", vec3.new(coords.y * roomSize + roomSize/2 + 8, 0.1, coords.x * roomSize + roomSize/2 + 8))
--CreatePrefabAtPosition("EnemyTurretAOE", vec3.new(coords.y * roomSize + roomSize/2 + 3, 0.5, coords.x * roomSize + roomSize/2 - 3))
--CreatePrefabAtPosition("EnemyFollowLookdir", vec3.new(coords.y * roomSize + roomSize/2 + 3, 0.5, coords.x * roomSize + roomSize/2))
--CreatePrefabAtPosition("EnemyTurretTarget", vec3.new(coords.y * roomSize + roomSize/2 - 3, 0.5, coords.x * roomSize + roomSize/2))
--CreatePrefabAtPosition("EnemyTurretCardinal", vec3.new(coords.y * roomSize + roomSize/2 + 3, 0.5, coords.x * roomSize + roomSize/2 + 3))
--CreatePrefabAtPosition("EnemyRunsawayLookDir", vec3.new(coords.y * roomSize + roomSize/2 - 3, 0.5, coords.x * roomSize + roomSize/2 - 3))

-- generate player in the entrance room
player = CreatePrefabAtPosition("Player", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2))

-- generate a spell in front of the player in the entrance room on the first floor
if(currentFloor == 0) then
    -- TODO instead of doing this we can spawn the player with a random starting spell each time
    local spell = CreatePrefabAtPosition("FireSpell", vec3.new(coords.y * roomSize + roomSize/2, 1.0, coords.x * roomSize + roomSize/2+5))
end

-- genetate boss in the boss room
local bossRoom = floor:GetBossIndex()
PopulateRoom(floor:GetRoomFromIndex(bossRoom))


-- roomCount is AllRooms - (Entrance & Boss) - (Treasure) - index
local roomCount = floor:GetRoomCount() - 2 - 1 - 1
for i=0, roomCount, 1 do
    PopulateRoom(floor:GetRegularRoom(i))
end

-- generate random valid pickup
CreatePickUpInRoom(floor:GetRegularRoom(roomCount + 1))
local airSpell = CreatePrefabAtPosition("AirSpell", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 - 10.0))
local earthSpell = CreatePrefabAtPosition("EarthSpell", vec3.new(coords.y * roomSize + roomSize/2 - 5, 0.5, coords.x * roomSize + roomSize/2 - 10.0))
local waterSpell = CreatePrefabAtPosition("WaterSpell", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 + 10.0))