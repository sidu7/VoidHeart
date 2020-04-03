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

function Update()
    local impulse = vec3.new()
    local direction = vec3.new();

    local front = vec3.new(0.0, 0.0, -1.0)
    local right = vec3.new(1.0, 0.0, 0.0)

    front.y = 0.0
    right.y = 0.0

	local playerStats = player:GetStats()

    local speed = vec3.new(playerStats.movementSpeed * playerStats.movementSpeedFactor,
							0.0,
							playerStats.movementSpeed * playerStats.movementSpeedFactor)

    local transform = gameObject:GetTransform()
    local xDirLeftStick = GetAxis(CONTROLLER["LX"])
    local zDirLeftStick = GetAxis(CONTROLLER["LY"])
    local rot = vec3.new(transform.rotation.x, transform.rotation.y, transform.rotation.z)
    --rot = VecNormalize(rot)
    if ((xDirLeftStick < -16000) or (xDirLeftStick > 16000) or (zDirLeftStick < -16000) or (zDirLeftStick > 16000)) then
        rot = CalculateRotation(xDirLeftStick, zDirLeftStick)
        
        local angle = rot.y
        angle = angle * math.pi / 180
        local xDir = math.sin(angle)
        local zDir = math.cos(angle)

        local magnitude = math.sqrt(xDir*xDir + zDir*zDir)
        xDir = xDir / magnitude
        zDir = zDir / magnitude
        impulse = vec3.new(xDir, 0, zDir)
    end

    -- look direction
    local xDirRightStick = GetAxis(CONTROLLER["RX"])
    local zDirRightStick = GetAxis(CONTROLLER["RY"])
    if ((xDirRightStick < -16000) or (xDirRightStick > 16000) or (zDirRightStick < -16000) or (zDirRightStick > 16000)) then
        rot = CalculateRotation(xDirRightStick, zDirRightStick)
    end

    local body = gameObject:GetBody()
    body:RotateBody(rot)
    
    impulse = impulse * speed
    	
    ApplyLinearImpulse(gameObject, impulse)


    -- Damp Overall Velocity and Rotation
    body.velocity = body.velocity - 0.8 * body.velocity 
    body.angularVelocity = body.angularVelocity - 0.8* body.angularVelocity;
    body.angularVelocity.x = 0
    body.angularVelocity.z = 0
end

Update()