function MoveInSinWave()
	local pos = gameObject:GetBody().position
	local oldPos = vec3.new(pos.x, pos.y, pos.z)
	-----------------------------------------
    -- playtesting vars
	local waterBossSinFactor = 8
	local waterBossZOffsetValue = 0.03

	local waterBossZOffsetLimit = 4 * math.pi
    -----------------------------------------
	
	if waterBossMoveDirection then
		waterBossZOffset = waterBossZOffset +waterBossZOffsetValue
		if waterBossZOffset > waterBossZOffsetLimit then
			waterBossMoveDirection = false
		end
		pos.z = waterBossZOffsetValue + pos.z
		pos.x = GetRoomCenterPosition().x +waterBossSinFactor* math.sin(waterBossZOffset)
	else 
		waterBossZOffset = waterBossZOffset - waterBossZOffsetValue
		if waterBossZOffset < -waterBossZOffsetLimit then
			waterBossMoveDirection = true
		end
		pos.z = pos.z - waterBossZOffsetValue
		pos.x = GetRoomCenterPosition().x +waterBossSinFactor* math.sin(waterBossZOffset - math.pi)
	end

	LookAt(oldPos, pos)
end

function LookAt(oldPos, pos)
	-- calculate direction
    local xDir = pos.x - oldPos.x
	local zDir = pos.z - oldPos.z
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
        gameObject:GetBody():RotateBody(rot)
    end
    if zDirNorm < 0 then 
	    rot = vec3.new(0.0, degree + 180, 0.0)
        gameObject:GetBody():RotateBody(rot)
    end
end

function MoveInMovingCircles()
	local roomCenterPos = GetRoomCenterPosition()
	local pos = gameObject:GetBody().position
	local oldPos = vec3.new(pos.x, pos.y, pos.z)
	-----------------------------------------
    -- playtesting vars
	local radiusCircle = 7
	local radiusCenterCircle = 5
    -----------------------------------------
	local thetaOffset = math.pi * 0.01
	local radiusThetaOffset = math.pi * 0.0005
	local rightCircleCenter = vec3.new(radiusCenterCircle * math.cos(waterBossRadiusTheta), 0, radiusCenterCircle * math.sin(math.pi + waterBossRadiusTheta))
	local leftCircleCenter = vec3.new(radiusCenterCircle * math.cos(waterBossRadiusTheta2), 0, radiusCenterCircle * math.sin(math.pi + waterBossRadiusTheta2))

	if(gameObject.tag == "Boss") then
		waterBossRadiusTheta = (waterBossRadiusTheta + radiusThetaOffset) % (2 * math.pi)
		waterBossTheta = (waterBossTheta + thetaOffset) % (2 * math.pi)
		pos.x = roomCenterPos.x + rightCircleCenter.x + radiusCircle * math.cos(waterBossTheta)
		pos.z = roomCenterPos.z + rightCircleCenter.z + radiusCircle * math.sin(math.pi + waterBossTheta)
	end
	
	if(gameObject.tag == "Boss2") then
		waterBossRadiusTheta2 = (waterBossRadiusTheta2 + radiusThetaOffset) % (2 * math.pi)
		waterBossTheta2 = (waterBossTheta2 + thetaOffset) % (2 * math.pi)
		pos.x = roomCenterPos.x + leftCircleCenter.x + radiusCircle * math.cos(waterBossTheta2)
		pos.z = roomCenterPos.z + leftCircleCenter.z + radiusCircle * math.sin(math.pi + waterBossTheta2)
	end
	
	LookAt(oldPos, pos)
end


function MoveInCircles()
	local roomCenterPos = GetRoomCenterPosition()
	local pos = gameObject:GetBody().position
	local oldPos = vec3.new(pos.x, pos.y, pos.z)
	-----------------------------------------
    -- playtesting vars
	local radiusCircle = 5
    -----------------------------------------
	local topCircleCenter = vec3.new(0, 0, -radiusCircle)
	local leftCircleCenter = vec3.new(-radiusCircle * math.cos(math.pi/6), 0, radiusCircle * math.sin(math.pi/6))
	local rightCircleCenter = vec3.new(radiusCircle * math.cos(math.pi/6), 0, radiusCircle * math.sin(math.pi/6))
	local topCircleTheta = math.pi * 1.5
	local leftCircleTheta = math.pi/6.0
	local rightCircleTheta = math.pi * (5.0/6.0)
	local thetaOffset = math.pi * 0.01
	local center = roomCenterPos + vec3.new(0, 0, 7)

	-- Set Circle to rotate around
	if waterBossCircleSelected then
		if waterBossCircle == 1 then
			waterBossTheta = topCircleTheta
			circleCenter = topCircleCenter
		elseif waterBossCircle == 2 then
			waterBossTheta = leftCircleTheta
			circleCenter = leftCircleCenter
		elseif waterBossCircle == 3 then
			waterBossTheta = rightCircleTheta
			circleCenter = rightCircleCenter
		end
		waterBossCircleSelected = false
	end

	-- increment theta
	waterBossTheta = (waterBossTheta + thetaOffset) % (2 * math.pi)

	-- check if one revolution is complete and select next circle
	if waterBossCircle == 1 and math.abs(waterBossTheta - topCircleTheta) < thetaOffset/2 then
		waterBossCircleSelected = true
		waterBossCircle = 2
	elseif waterBossCircle == 2 and math.abs(waterBossTheta - leftCircleTheta) < thetaOffset/2 then
		waterBossCircleSelected = true
		waterBossCircle = 3
	elseif waterBossCircle == 3 and math.abs(waterBossTheta - rightCircleTheta) < thetaOffset/2 then
		waterBossCircleSelected = true
		waterBossCircle = 1
	end
	
	-- set boss position on circle
	pos.x = roomCenterPos.x + circleCenter.x + radiusCircle * math.cos(waterBossTheta)
	pos.z = roomCenterPos.z + circleCenter.z + radiusCircle * math.sin(math.pi + waterBossTheta)

	LookAt(oldPos, pos)
end

function MoveToCenter()
  	local speed = 5.0
    
    -- collision handling
    local transform = gameObject:GetTransform()
	local position = transform.position
    -- get room center pos
	local targetPos = GetRoomCenterPosition()
    -- calculate direction
    local xDir = targetPos.x - position.x
	local zDir = targetPos.z - position.z
	local dirLength = math.sqrt(xDir*xDir + zDir*zDir)
	local xDirNorm = xDir / dirLength
	local zDirNorm = zDir / dirLength
	
    if (dirLength > 0.1) then -- walking to the center
        -- look at the target
        local body = gameObject:GetBody()
        local rot = vec3.new(0.0, 0.0, 0.0)
        local tangent = xDirNorm / zDirNorm
        local radians = math.atan(tangent)
        local degree = radians * 180 / math.pi
        if zDirNorm >= 0 then  
	        rot = vec3.new(0.0, degree, 0.0)
            body:RotateBody(rot)
        end
        if zDirNorm < 0 then 
	        rot = vec3.new(0.0, degree + 180, 0.0)
            body:RotateBody(rot)
        end
    
        -- setting the velocity
        body.velocity = speed * vec3.new(xDirNorm, 0.0, zDirNorm)
    else
        -- once at the center signal to attacking script to spawn rocks
        local attack = gameObject:GetAttack()
        attack.shouldAttack2 = not attack.shouldAttack2
    end
end

function PhaseOneMovement()
	MoveInSinWave()
	local attack = gameObject:GetAttack()
	attack.shouldAttack2 = false
	attack.shouldAttack = true
end

function PhaseTwoMovement()
	local attack = gameObject:GetAttack()
    if attack.shouldAttack2 == true then
		MoveInCircles()
	else
		MoveToCenter()
	end
end

function SetupPhaseThree()
	-- Create another copy of boss
	local attack = gameObject:GetAttack()
	waterBoss2 = CreatePrefabAtPosition("EnemyBossWater", GetRoomCenterPosition() + vec3.new(2,2.5,0))
	waterBoss2.tag = "Boss2"
	waterBoss2:GetHealth().hitPoints = gameObject:GetHealth().hitPoints
	waterBoss2:GetAttack().shouldAttack2 = false -- used for MovingToCenter
	waterBoss2:GetAttack().shouldAttack = false -- used for SetupPhaseThree

	-- Add new boss to room enemies list
	local enemies = GetDungeonFloor(currentFloor):GetRoomFromIndex(currentRoom).Enemies
	enemies[#enemies + 1] = waterBoss2 -- recommended idiom for push_back in lua

	-- Setup Circle positions and radii
	waterBossRadiusTheta2 = math.pi
	waterBossTheta2 = 0

	waterBossRadiusTheta = 0
	waterBossTheta = math.pi
	gameObject:GetBody().position = GetRoomCenterPosition() + vec3.new(-2,2.5,0)

	attack.shouldAttack = false
end

function PhaseThreeMovement()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack2 == true then
		MoveToCenter()
	elseif attack.shouldAttack == true then
		SetupPhaseThree() -- Setup values for moving in circles
	else
		MoveInMovingCircles()
	end
end

function Update()
    -----------------------------------------
    -- playtesting vars
	local maxHealth = 150
    -----------------------------------------
	
	local health = gameObject:GetHealth()
    local hitPoints = health.hitPoints
	
	-- set vertical velocity to zero
	gameObject:GetBody().velocity.y = 0

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