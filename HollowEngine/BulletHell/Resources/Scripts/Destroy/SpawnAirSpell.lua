function SpawnAirSpell ()
	local go = CreatePrefabAtPosition("AirSpell", gameObject:GetTransform().position)
	go:GetBody().position.y = player:GetBody().position.y
	go:GetTransform().position.y = go:GetBody().position.y

	-- Deactivate all pillars in the room
	local floor = GetDungeonFloor(currentFloor)
	local currentRoomObj = floor:GetRoomFromIndex(currentRoom)
	local pillars = currentRoomObj.obstacles
	for k=1,#pillars do
		pillars[k]:GetAttack().isActive = false
	end
end

SpawnAirSpell()