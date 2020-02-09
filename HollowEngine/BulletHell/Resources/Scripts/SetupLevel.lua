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
print(roomCount)
for i=0, roomCount, 1 do
    print(i)
    CreateEnemiesInRoom(firstFloor:GetRegularRoom(i))
end

-- generate random valid pickup
CreatePickUpInRoom(firstFloor:GetRegularRoom(roomCount + 1))
local airSpell = CreatePrefabAtPosition("AirSpell", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 - 10.0))
local earthSpell = CreatePrefabAtPosition("EarthSpell", vec3.new(coords.y * roomSize + roomSize/2 - 5, 0.5, coords.x * roomSize + roomSize/2 - 10.0))
local waterSpell = CreatePrefabAtPosition("WaterSpell", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 + 10.0))