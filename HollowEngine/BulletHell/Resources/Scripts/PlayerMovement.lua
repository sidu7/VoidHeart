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

    local speed = vec3.new(1000.0, 0.0, 1000.0)

    local transform = gameObject:GetTransform()
    local xDirLeftStick = GetAxis(CONTROLLER["LX"])
    local zDirLeftStick = GetAxis(CONTROLLER["LY"])
    local rot = transform.rotation

    if ((xDirLeftStick < -16000) or (xDirLeftStick > 16000) or (zDirLeftStick < -16000) or (zDirLeftStick > 16000)) then

        rot = CalculateRotation(xDirLeftStick, zDirLeftStick)

        if IsKeyPressed("W") or zDirLeftStick < -16000 then
	        impulse = impulse + front;
        end
        if IsKeyPressed("S") or zDirLeftStick > 16000 then
	        impulse = impulse - front;
        end
        if IsKeyPressed("A") or xDirLeftStick < -16000 then
	        impulse = impulse - right;
        end
        if IsKeyPressed("D") or xDirLeftStick > 16000 then
	        impulse = impulse + right;
        end
    end

    -- look direction
    local xDirRightStick = GetAxis(CONTROLLER["RX"])
    local zDirRightStick = GetAxis(CONTROLLER["RY"])
    if ((xDirRightStick < -16000) or (xDirRightStick > 16000) or (zDirRightStick < -16000) or (zDirRightStick > 16000)) then
        rot = CalculateRotation(xDirRightStick, zDirRightStick)
    end

    transform:Rotate(rot)

-- Apply any movement debuffs
local movement = gameObject:GetMovement()

impulse = impulse * speed * movement.moveDebuffFactor;

movement.moveDebuffFactor = 1.0; -- reset debuff for this frame

    ApplyLinearImpulse(gameObject, impulse)

    local body = gameObject:GetBody()

    -- Damp Overall Velocity and Rotation
    body.velocity = body.velocity - 0.8 * body.velocity 
    body.angularVelocity = body.angularVelocity - 0.8* body.angularVelocity;
end

Update()