local firstFloor = GetDungeonFloor(currentFloor)
local coords = firstFloor:GetEntrance():GetCoords()

currentRoom = firstFloor:GetEntranceIndex()
player = CreatePrefabAtPosition("Player", vec3.new(coords.y * roomSize + roomSize/2, 2.0, coords.x * roomSize + roomSize/2))

coords = firstFloor:GetRoomFromIndex(1):GetCoords()
spell = CreatePrefabAtPosition("FireballSpell", vec3.new(coords.y * roomSize + roomSize/2, 1.0, coords.x * roomSize + roomSize/2))