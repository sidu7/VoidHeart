function UpdateCamera()
	-----------------------------------------
    -- playtesting vars
	local xOffset = 0.0
	local yOffset = 17.0
	local zOffset = roomSize/2 + 1
	local cameraFront = vec3.new(0.0, -1.4, -1.0)
	-----------------------------------------
	
	-- Get camera object components
	local transform = gameObject:GetTransform()
	local camera = gameObject:GetCamera()
	
	-- Set camera front direction
	camera.frontDirection = cameraFront 
	
	-- Find camera offset based on current room
	local floor = GetDungeonFloor(currentFloor)
	local coords = floor:GetRoomFromIndex(currentRoom):GetCoords()
	local roomOffset = vec3.new(coords.y * roomSize + roomSize/2 + xOffset, yOffset, coords.x * roomSize + roomSize/2 + zOffset)
	local desiredPosition = roomOffset 
	
	-- LERP to desired position
	local t = camera.LERPFactor
	local d = camera.PreviousPosition + t * (desiredPosition - camera.PreviousPosition)
	-- Set new position
	transform.position = d
end

UpdateCamera()