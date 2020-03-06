function SetLightPosition()
	-----------------------------------------
    -- playtesting vars
	local positionOffset = vec3.new(10.0, 10.0, 25.0)
	local lookAtOffset = vec3.new(0.0, 0.0, 0.0)
	local t = 0.05
	-----------------------------------------	
	local transform = gameObject:GetTransform()
	
	--player = CreatePrefabAtPosition("Player", vec3.new(coords.y * roomSize + roomSize/2, 0.5, coords.x * roomSize + roomSize/2))
	local floor = GetDungeonFloor(currentFloor)
	local currentRoomObj = floor:GetRoomFromIndex(currentRoom)
	local coords = currentRoomObj:GetCoords()
	-- Light position is based on current room
	position = vec3.new(coords.y * roomSize + roomSize/2, 0.0, coords.x * roomSize + roomSize/2) + positionOffset
	transform.position = transform.position + t * (position - transform.position)
	
	-- Set light to look at center of room
	
	local light = gameObject:GetLight()
	local lookAtPos = vec3.new(coords.y * roomSize + roomSize/2, 0.0, coords.x * roomSize + roomSize/2) + lookAtOffset
	--local lookAt = vec3.new(0.0, 0.0, 0.0)
	--lookAt = lookAtPos + lookAtOffset
	light.lookAt = light.lookAt + t * (lookAtPos - light.lookAt) 
end

function UpdateLight()
	SetLightPosition()
end

UpdateLight()