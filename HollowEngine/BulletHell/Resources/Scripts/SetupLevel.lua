local firstFloor = GetDungeonFloor(currentFloor)

currentRoom = firstFloor:GetEntranceIndex()
local coords = firstFloor:GetEntrance():GetCoords()
player = CreatePrefabAtPosition("Player", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2))

-- generate a spell in front of the player in the entrance room on the first floor
if(currentFloor == 0) then
    local spell = CreatePrefabAtPosition("FireSpell", vec3.new(coords.y * roomSize + roomSize/2, 1.0, coords.x * roomSize + roomSize/2+5))
end

-- roomCount is AllRooms - (Entrance & Boss) - (Treasure) - index
local roomCount = firstFloor:GetRoomCount() - 2 - 1 - 1
for i=0, roomCount do
    coords = firstFloor:GetRoomFromIndex(i):GetCoords()
	-- get a random enemy type and set count and difficulty according to floor
    CreatePrefabAtPosition("Enemy", vec3.new(coords.y * roomSize + roomSize/2 + 10, 1.0, coords.x * roomSize + roomSize/2))
    CreatePrefabAtPosition("Enemy", vec3.new(coords.y * roomSize + roomSize/2 - 10, 1.0, coords.x * roomSize + roomSize/2))
end

-- generate random valid pickup
coords = firstFloor:GetRoomFromIndex(roomCount):GetCoords()
local airSpell = CreatePrefabAtPosition("AirSpell", vec3.new(coords.y * roomSize + roomSize/2, 1.0, coords.x * roomSize + roomSize/2))
