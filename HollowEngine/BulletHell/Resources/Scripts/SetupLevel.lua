local firstFloor = GetDungeonFloor(currentFloor)
local coords = firstFloor:GetEntrance():GetCoords()

currentRoom = firstFloor:GetEntranceIndex()
player = CreatePrefabAtPosition("Player", vec3.new(coords.y * roomSize + roomSize/2, 1.0, coords.x * roomSize + roomSize/2))
CreatePrefabAtPosition("EnemyFollowLookdir", vec3.new(coords.y * roomSize + roomSize/2 + 3, 0.5, coords.x * roomSize + roomSize/2))
CreatePrefabAtPosition("EnemyTurretTarget", vec3.new(coords.y * roomSize + roomSize/2 - 3, 0.5, coords.x * roomSize + roomSize/2))
CreatePrefabAtPosition("EnemyTurretCardinal", vec3.new(coords.y * roomSize + roomSize/2 + 3, 0.5, coords.x * roomSize + roomSize/2 + 3))
CreatePrefabAtPosition("EnemyRunsawayLookDir", vec3.new(coords.y * roomSize + roomSize/2 - 3, 0.5, coords.x * roomSize + roomSize/2 - 3))

--local coords = firstFloor:GetRoomFromIndex(1):GetCoords()
local spell = CreatePrefabAtPosition("FireSpell", vec3.new(coords.y * roomSize + roomSize/2 +5, 1.0, coords.x * roomSize + roomSize/2+5))

local airSpell = CreatePrefabAtPosition("AirSpell", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 - 10.0))
local earthSpell = CreatePrefabAtPosition("EarthSpell", vec3.new(coords.y * roomSize + roomSize/2 - 5, 0.5, coords.x * roomSize + roomSize/2 - 10.0))
local waterSpell = CreatePrefabAtPosition("WaterSpell", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2 + 10.0))