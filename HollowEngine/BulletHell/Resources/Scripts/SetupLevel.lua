local firstFloor = GetDungeonFloor(currentFloor)
local coords = firstFloor:GetEntrance():GetCoords()

player = CreatePrefabAtPosition("Player", vec3.new(coords.y * 15 + 7.5, 2.0, coords.x * 15 + 7.5))

coords = firstFloor:GetRoomFromIndex(5):GetCoords()
spell = CreatePrefabAtPosition("FireballSpell", vec3.new(coords.y * 15 + 7.5, 1.0, coords.x * 15 + 7.5))