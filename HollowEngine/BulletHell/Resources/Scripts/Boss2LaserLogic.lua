function CalculateRotation(xDir, zDir)
    local dirLength = math.sqrt(xDir*xDir + zDir*zDir)
    local xDirNorm = xDir / dirLength
    local zDirNorm = zDir / dirLength

    local tangent = xDirNorm / zDirNorm
    local radians = math.atan(tangent)
    local degree = radians * 180 / math.pi
    local rot = vec3.new()
    if zDirNorm >= 0 then  
	    rot = vec3.new(0.0, degree, 0.0)
    end
    if zDirNorm < 0 then 
	    rot = vec3.new(0.0, degree + 180, 0.0)
    end

    return rot
end

function Rotate()
	-----------------------------------------
    -- playtesting vars
    -----------------------------------------
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

    local xDir = position.x - centerPos.x
	local zDir = position.z - centerPos.z
	local dirLength = math.sqrt(xDir*xDir + zDir*zDir)
	local xDirNorm = xDir / dirLength
	local zDirNorm = zDir / dirLength

    local rot = vec3.new(0.0, 0.0, 0.0)
    local tangent = xDirNorm / zDirNorm
    local radians = math.atan(tangent)
    local degree = radians * 180 / math.pi
    if zDirNorm >= 0 then  
	    rot = vec3.new(0.0, degree, 0.0)
        transform:Rotate(rot)
    end
    if zDirNorm < 0 then 
	    rot = vec3.new(0.0, degree + 180, 0.0)
        transform:Rotate(rot)
    end


	--[[ temp: --]]
	local particle = gameObject:GetParticleEmitter()
	if particle ~= nil then
		particle.extraData.z = particle.extraData.z + GetFrameTime()
		particle.direction = VecNormalize(transform.position - centerPos)
		particle.center = centerPos
	end
	-- temp --]]
        
end

function ScaleZ()
	-----------------------------------------
    -- playtesting vars
    -----------------------------------------
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
    local dirFromCenter = position - centerPos
    local dirNorm = VecNormalize(dirFromCenter)
    local start = centerPos + dirNorm
	local depth = DCastRay(start, dirNorm)
	
	--local body = gameObject:GetBody()
	--local front = VecNormalize(gameObject:GetTransform():forward())
    --local offset = depth / 2
    --local distDir = vec3.new(dirNorm.x * offset, dirNorm.y * offset, dirNorm.z * offset)
    --local newPosition = vec3.new(0,0,0)
    --newPosition = centerPos + distDir

    --position = vec3.new(newPosition.x, newPosition.y, newPosition.z)

	--body.position = position--start + vec3.new(dirNorm.x*(depth/2.0), 0.0, dirNorm.z*(depth/2.0))

	-- Update shape radius for physics
    --transform.position = body.position
	transform.scale.z = 20
    
	--[[]]local particle = gameObject:GetParticleEmitter()
	if particle ~= nil then
		particle.extraData.y = depth
	end--[[]]
	ChangeRadius(gameObject)
end


function Translate()
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
    --position = centerPos + radiusDirNorm * radius 
    
    -- radial
    local offset = radiusDirNorm * (transform.scale.z * 0.5 + 1)
    offset.y = playerPos.y 
    --local offset = vec3.new(0,0,transform.scale.z)
    --local offset = (transform.scale.z / 2) * radiusDirNorm
   -- position = position + (transform.scale.z / 2) * radiusDirNorm
    radiusDir.y = 0
    local body = gameObject:GetBody()
    body.position = centerPos + offset
    --position = vec3.new(newPosition.x, newPosition.y, newPosition.z)

    --rotational
    local upDir = vec3.new(0,1,0)
    local movingDir = VecCross(upDir, radiusDir)
    local movingDirNorm = VecNormalize(movingDir)
    
    transform.position.y = playerPos.y

    --local halfScale = transform.scale.z / 2
    --local radiusDirScaled = vec3.new(halfScale * radiusDirNorm.x, halfScale * radiusDirNorm.y, halfScale * radiusDirNorm.z)
    --body.position = centerPos + radiusDirScaled
    --position = body.position
    body.velocity = movingDirNorm * rotationSpeed
end

function Update()
    Rotate()

    ScaleZ()

    Translate()
end

Update()