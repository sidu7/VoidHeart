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
	local transform = gameObject:GetTransform()
	
	local xDirLeftStick = GetAxis(CONTROLLER["LX"])
    local zDirLeftStick = GetAxis(CONTROLLER["LY"])
    local playerTransform = player:GetTransform()
    local rot = vec3.new(playerTransform.rotation.x, playerTransform.rotation.y, playerTransform.rotation.z)

    if ((xDirLeftStick < -16000) or (xDirLeftStick > 16000) or (zDirLeftStick < -16000) or (zDirLeftStick > 16000)) then
        rot = CalculateRotation(xDirLeftStick, zDirLeftStick)

    end
	
	 -- look direction
    local xDirRightStick = GetAxis(CONTROLLER["RX"])
    local zDirRightStick = GetAxis(CONTROLLER["RY"])
    if ((xDirRightStick < -16000) or (xDirRightStick > 16000) or (zDirRightStick < -16000) or (zDirRightStick > 16000)) then
        rot = CalculateRotation(xDirRightStick, zDirRightStick)
    end

    transform:Rotate(rot)
        
end

Rotate()