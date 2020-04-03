function MoveInSinWave()
	local pos = gameObject:GetBody().position
	local oldPos = vec3.new(pos.x, pos.y, pos.z)
	-----------------------------------------
    -- playtesting vars
	local waterBossSinFactor = 6
	local waterBossZOffsetValue = 0.05
	local waterBossZOffsetLimit = 3 * math.pi
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
		end
		if waterBossCircle == 2 then
			waterBossTheta = leftCircleTheta
			circleCenter = leftCircleCenter
		end
		if waterBossCircle == 3 then
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
	end
	if waterBossCircle == 2 and math.abs(waterBossTheta - leftCircleTheta) < thetaOffset/2 then
		waterBossCircleSelected = true
		waterBossCircle = 3
	end
	if waterBossCircle == 3 and math.abs(waterBossTheta - rightCircleTheta) < thetaOffset/2 then
		waterBossCircleSelected = true
		waterBossCircle = 1
	end
	
	-- set boss position on circle
	pos.x = roomCenterPos.x + circleCenter.x + radiusCircle * math.cos(waterBossTheta)
	pos.z = roomCenterPos.z + circleCenter.z + radiusCircle * math.sin(math.pi + waterBossTheta)

	LookAt(oldPos, pos)
end


function PhaseOneMovement()
	MoveInSinWave()
end

function PhaseTwoMovement()
	MoveInCircles()
end

function PhaseThreeMovement()

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