function Update()
    local radius = 2
    local rotationSpeed = 5
    ------------------------
    if (player == nil or player.isActive == false) then
        return
    end
    local playerTransform = player:GetTransform()
    local playerPos = playerTransform.position

    local room = GetDungeonFloor(currentFloor):GetRoomFromIndex(currentRoom)
	local roomCoords = room:GetCoords()
    local centerX = (roomCoords.y + roomCoords.y + 1) * roomSize / 2 
    local centerZ = (roomCoords.x + roomCoords.x + 1) * roomSize / 2 
    local centerPos = vec3.new(centerX, playerPos.y, centerZ) 
    
    local transform = gameObject:GetTransform()
    local position = transform.position
    
    local radiusDir = position - centerPos
    local radiusDirNorm = VecNormalize(radiusDir)
    position = centerPos + radiusDirNorm * radius
    radiusDir.y = 0
    local upDir = vec3.new(0,1,0)
    local movingDir = VecCross(upDir, radiusDir)
    local movingDirNorm = VecNormalize(movingDir)
    
    position.y = playerPos.y
    local body = gameObject:GetBody()
    body.position = position
    body.velocity = movingDirNorm * rotationSpeed
end

Update()