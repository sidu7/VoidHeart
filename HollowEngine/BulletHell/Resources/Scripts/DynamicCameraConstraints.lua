function ApplyDynamicConstraints()
	-----------------------------------------
    -- playtesting vars
	--7 constraint for x and z
	local XConstraint = 7
	local ZMinConstraint = 2
	local ZMaxConstraint = 14
	local t = 0.08
	-----------------------------------------	
	local transform = gameObject:GetTransform()
	
	-- Get room coordinates
	local floor = GetDungeonFloor(currentFloor)
	--local currentRoomObj = floor:GetRoomFromIndex(currentRoom)
	local coords = floor:GetRoomFromIndex(currentRoom):GetCoords()
	
	-- Apply constraints to position
	local position = vec3.new(transform.position.x,transform.position.y,transform.position.z)
	-- Calculate min and max positions based on room position
	local xMin = coords.y * roomSize + roomSize/2 - XConstraint
	local xMax = coords.y * roomSize + roomSize/2 + XConstraint
	local zMin = coords.x * roomSize + roomSize/2 - ZMinConstraint
	local zMax = coords.x * roomSize + roomSize/2 + ZMaxConstraint
	
	position.x = math.max(xMin, math.min(xMax, position.x))
	position.z = math.max(zMin, math.min(zMax, position.z))
	
	--pCamera->mPreviousPosition + t * (desiredPosition - pCamera->mPreviousPosition);
	transform.position = transform.position + t * (position - transform.position)
end

ApplyDynamicConstraints()