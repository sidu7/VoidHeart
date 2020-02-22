function SetCameraPosition()
	-----------------------------------------
    -- playtesting vars
	local cameraFront = vec3.new(0.0, -1.2, -1.0)
	-----------------------------------------	
	local camera = gameObject:GetCamera()
	
	-- Set camera front direction
	camera.frontDirection = cameraFront 
	
	-- Get the current room/enemies
	local floor = GetDungeonFloor(currentFloor)
	local currentRoomObj = floor:GetRoomFromIndex(currentRoom)
	local enemies = currentRoomObj.Enemies
	
	local i = 0
	for k=1,#enemies do
		camera.FocusPositions[k] = (enemies[k]:GetTransform())
		i = k
	end
	camera.FocusPositions[i+1] = player:GetTransform()
end

function UpdateCamera()
	SetCameraPosition()
end

UpdateCamera()