-- Setups a Floor of the Dungeon 
local floor = GetDungeonFloor(currentFloor)
local floorNum = floor:GetFloorNum()

-- update current room to the dungeon entrance
currentRoom = floor:GetEntranceIndex()

local coords = floor:GetEntrance():GetCoords()
CreatePrefabAtPosition("EnemyBoss2", vec3.new(coords.y * roomSize + roomSize/2 + 8, 1, coords.x * roomSize + roomSize/2 + 8))
--CreatePrefabAtPosition("EnemyTurretAOE", vec3.new(coords.y * roomSize + roomSize/2 + 3, 0.5, coords.x * roomSize + roomSize/2 - 3))
--CreatePrefabAtPosition("EnemyFollowLookdir", vec3.new(coords.y * roomSize + roomSize/2 + 3, 0.5, coords.x * roomSize + roomSize/2))
--CreatePrefabAtPosition("EnemyTurretTarget", vec3.new(coords.y * roomSize + roomSize/2 - 3, 0.5, coords.x * roomSize + roomSize/2))
--CreatePrefabAtPosition("EnemyTurretCardinal", vec3.new(coords.y * roomSize + roomSize/2 + 3, 0.5, coords.x * roomSize + roomSize/2 + 3))
--CreatePrefabAtPosition("EnemyRunsawayLookDir", vec3.new(coords.y * roomSize + roomSize/2 - 3, 0.5, coords.x * roomSize + roomSize/2 - 3))

-- generate player in the entrance room
--player = CreatePrefabAtPosition("Player", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2))
--player = CreatePrefabAtPosition("Player", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2))
player:GetBody().position = vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2)
local gameObjectPath = "Resources/Json data/Camera.json"
camera = CreateGameObject(gameObjectPath)

-- generate a spell in front of the player in the entrance room on the first floor
if(currentFloor == 0) then
    -- TODO instead of doing this we can spawn the player with a random starting spell each time
    local spell = CreatePrefabAtPosition("FireSpell", vec3.new(coords.y * roomSize + roomSize/2, 1.0, coords.x * roomSize + roomSize/2+5))
end

-- genetate boss in the boss room
local bossRoom = floor:GetBossIndex()
PopulateRoom(floor:GetRoomFromIndex(bossRoom))


-- roomCount is AllRooms - (Entrance & Boss) - (Treasure) - index
local roomCount = floor:GetRoomCount() - 2 - 1 - floorNum
for i=0, roomCount, 1 do
    PopulateRoom(floor:GetRegularRoom(i))
end

-- generate random valid pickup
for i=0, floorNum, 1 do
	--CreatePickUpInRoom(floor:GetRegularRoom(roomCount + i))
end

-- Comment this out if you dont want pickups in starting room
---[[
local hpPickup = CreatePrefabAtPosition("Pickup_HP", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 - 10.0))
local speedPickup = CreatePrefabAtPosition("Pickup_Speed", vec3.new(coords.y * roomSize + roomSize/2 - 5, 0.5, coords.x * roomSize + roomSize/2 - 10.0))
local invinciblePickup = CreatePrefabAtPosition("Pickup_Invincible", vec3.new(coords.y * roomSize + roomSize/2 + 5, 1.5, coords.x * roomSize + roomSize/2 + 8.0))
local rofPickup = CreatePrefabAtPosition("Pickup_RateOfFire", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 - 15.0))
--]]

-- Comment this out if you dont want all spells in starting room
---[[
local fireSpell = CreatePrefabAtPosition("FireSpell", vec3.new(coords.y * roomSize + roomSize/2 + 1.0, 0.5, coords.x * roomSize + roomSize/2 + 10.0))
local airSpell = CreatePrefabAtPosition("AirSpell", vec3.new(coords.y * roomSize + roomSize/2 - 1.0, 0.5, coords.x * roomSize + roomSize/2 + 10.0))
local earthSpell = CreatePrefabAtPosition("EarthSpell", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 + 11.0))
local waterSpell = CreatePrefabAtPosition("WaterSpell", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 + 9.0))
--]]