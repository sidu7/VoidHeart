-- Setups a Floor of the Dungeon 
local floor = GetDungeonFloor(currentFloor)
local floorNum = floor:GetFloorNum()

-- update current room to the dungeon entrance
currentRoom = floor:GetEntranceIndex()

local coords = floor:GetEntrance():GetCoords()
--CreatePrefabAtPosition("EnemyBoss2", vec3.new(coords.y * roomSize + roomSize/2 + 8, 1, coords.x * roomSize + roomSize/2 + 8))

-- Put player in the entrance room
player:GetBody().position = vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2)

-- Generate a spell in front of the player in the entrance room on the first floor
if(currentFloor == 0) then
	local spellString = ""
	local lastSpell = GetSpellOrder()[4]
	print("Last Spell")
	print(lastSpell)
	if(lastSpell == 1) then
		spellString = "FireSpell"
	elseif(lastSpell == 2) then
		spellString = "AirSpell"
	elseif(lastSpell == 3) then
		spellString = "EarthSpell"
	else
		spellString = "WaterSpell"
	end
    local spell = CreatePrefabAtPosition(spellString, vec3.new(coords.y * roomSize + roomSize/2, 1.0, coords.x * roomSize + roomSize/2+5))

	-- Lockdown the room until the spell is collected
	GetDungeonFloor(currentFloor):GetRoomFromIndex(currentRoom):LockDownRoom()
end

-- Generate boss in the boss room
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
--[[
local hpPickup = CreatePrefabAtPosition("Pickup_HP", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 - 10.0))
local speedPickup = CreatePrefabAtPosition("Pickup_Speed", vec3.new(coords.y * roomSize + roomSize/2 - 5, 0.5, coords.x * roomSize + roomSize/2 - 10.0))
local invinciblePickup = CreatePrefabAtPosition("Pickup_Invincible", vec3.new(coords.y * roomSize + roomSize/2 + 5, 1.5, coords.x * roomSize + roomSize/2 + 8.0))
local rofPickup = CreatePrefabAtPosition("Pickup_RateOfFire", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 - 15.0))
--]]

-- Comment this out if you dont want all spells in starting room
--[[
local fireSpell = CreatePrefabAtPosition("FireSpell", vec3.new(coords.y * roomSize + roomSize/2 + 1.0, 0.5, coords.x * roomSize + roomSize/2 + 10.0))
local airSpell = CreatePrefabAtPosition("AirSpell", vec3.new(coords.y * roomSize + roomSize/2 - 1.0, 0.5, coords.x * roomSize + roomSize/2 + 10.0))
local earthSpell = CreatePrefabAtPosition("EarthSpell", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 + 11.0))
local waterSpell = CreatePrefabAtPosition("WaterSpell", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 + 9.0))
--]]