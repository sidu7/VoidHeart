function FollowTarget(target, speedMod)
    -----------------------------------------
    -- playtesting vars
	local enemySpeed = 2.0 * speedMod
    -----------------------------------------
    local transform = gameObject:GetTransform()
	local position = transform.position

    --  acquire target 
    --local target = player
    if (target == nil or target.isActive == false) then
        return
    end
    local targetTransform = target:GetTransform()
    local targetPos = targetTransform.position
    
    -- calculate direction
    local xDir = targetPos.x - position.x
	local zDir = targetPos.z - position.z
	local dirLength = math.sqrt(xDir*xDir + zDir*zDir)
	local xDirNorm = xDir / dirLength
	local zDirNorm = zDir / dirLength
	
    -- look at the target
    local rot = vec3.new(0.0, 0.0, 0.0)
    local tangent = xDirNorm / zDirNorm
    local radians = math.atan(tangent)
    local degree = radians * 180 / math.pi
    if zDirNorm >= 0 then  
	    rot = vec3.new(0.0, degree, 0.0)
    else
	    rot = vec3.new(0.0, degree + 180, 0.0)
    end
    transform:Rotate(rot)
    
    -- setting the velocity
    local body = gameObject:GetBody()
    body.velocity = vec3.new(xDirNorm, 0.0, zDirNorm)
    body.velocity = body.velocity * enemySpeed

    -- collision handling
    if (dirLength < 2.0) then
        local impulseStrength = vec3.new(1000, 0, 1000)
        local impulse = vec3.new(xDirNorm, 0.0, zDirNorm)
        impulse  = impulse * impulseStrength
        ApplyLinearImpulse(player, impulse)

        local impulseReverse = vec3.new(-xDirNorm, 0.0, -zDirNorm)
        impulseReverse = impulseReverse * impulseStrength
        ApplyLinearImpulse(gameObject, impulseReverse)
    end
end

function SetCenterPillar()
-- Get center pillar
	local floor = GetDungeonFloor(currentFloor)
	local currentRoomObj = floor:GetRoomFromIndex(currentRoom)
	local pillars = currentRoomObj.obstacles
	
	-- Assumes center pillar is first obstacle in the room in JSON file
	centerPillar = pillars[1]
end

function MoveTowardsPillar(target, speedMod)
	--FollowTarget(pillar, false, speedMod)
	-----------------------------------------
    -- playtesting vars
	local enemySpeed = 2.0 * speedMod
    -----------------------------------------
    local transform = gameObject:GetTransform()
	local position = transform.position

    --  acquire target 
    --local target = player
    if (target == nil or target.isActive == false) then
        return
    end
    local targetTransform = target:GetTransform()
    local targetPos = targetTransform.position
    
    -- calculate direction
    local xDir = targetPos.x - position.x
	local yDir = targetPos.y - position.y
	local zDir = targetPos.z - position.z
	local dirLength = math.sqrt(xDir*xDir + yDir*yDir + zDir*zDir)
	local xDirNorm = xDir / dirLength
	local yDirNorm = yDir / dirLength
	local zDirNorm = zDir / dirLength
	
    -- look at the pillar
    local rot = vec3.new(0.0, 0.0, 0.0)
    local tangent = xDirNorm / zDirNorm
    local radians = math.atan(tangent)
    local degree = radians * 180 / math.pi
    if zDirNorm >= 0 then  
	    rot = vec3.new(0.0, degree, 0.0)
    else
	    rot = vec3.new(0.0, degree + 180, 0.0)
    end
    transform:Rotate(rot)
    
    -- setting the velocity
    local body = gameObject:GetBody()
    body.velocity = vec3.new(xDirNorm, yDirNorm, zDirNorm)
    body.velocity = body.velocity * enemySpeed
end

function PhaseOneMovement()
	FollowTarget(player, 1.0)
end

function PhaseTwoMovement()
	SetCenterPillar()
	if closestPillar then
		MoveTowardsPillar(closestPillar, 6.0)
	else
		MoveTowardsPillar(centerPillar, 1.0)
	end
end

function PhaseThreeMovement()
	if closestPillar then
		MoveTowardsPillar(closestPillar, 3.0)
	else
		FollowTarget(player, 1.5)
	end
end

function Update()
    -----------------------------------------
    -- playtesting vars
	local maxHealth = 200
    -----------------------------------------
	
	local health = gameObject:GetHealth()
    local hitPoints = health.hitPoints
	
	if(hitPoints >= maxHealth * 2.0 / 3.0) then
		-- First phase
		PhaseOneMovement()
	elseif(hitPoints >= maxHealth / 3.0) then
		-- Second phase
		PhaseTwoMovement()
	else
		-- Third phase
		PhaseThreeMovement()
	end
end

Update()