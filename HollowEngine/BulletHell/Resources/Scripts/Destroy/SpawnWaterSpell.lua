function SpawnWaterSpell ()
	-- Check if room is empty
	local floor = GetDungeonFloor(currentFloor)
	local currentRoomObj = floor:GetRoomFromIndex(currentRoom)
	local enemies = currentRoomObj.Enemies

	if #enemies == 0 and waterBossSpawnSpell == false then
		local go = CreatePrefabAtPosition("WaterSpell", gameObject:GetTransform().position)
		go:GetBody().position.y = player:GetBody().position.y
		go:GetTransform().position.y = go:GetBody().position.y
		waterBossSpawnSpell = true
	end
end

SpawnWaterSpell()