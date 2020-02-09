function MoveInCirle()
    -----------------------------------------
    -- playtesting vars
	local minDist = 6   -- keep the distance from player
    local velocity = 5  -- how fast too circle around player
    -----------------------------------------

	local transform = gameObject:GetTransform()
	local selfPos = transform.position
	local body = gameObject:GetBody()

	-- Get Player
	local target = player
    if (target == nil or target.isActive == false) then
        return
    end
    local targetTransform = target:GetTransform()
    local targetPos = targetTransform.position

    -- calculate direction
    local xDir = targetPos.x - selfPos.x
	local zDir = targetPos.z - selfPos.z
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
        transform:Rotate(rot)
    end
    if zDirNorm < 0 then 
	    rot = vec3.new(0.0, degree + 180, 0.0)
        transform:Rotate(rot)
    end

	-- Get Direction to move in circle
	local playerVec = targetPos - selfPos
	local Y = vec3.new(0,1,0)
	local direction = VecNormalize(VecCross(Y,playerVec));
	-- if enemy collides with wall reverse direction VecCross(Y,playerVec)
	-- need event for WALL ENEMY collision
	
	-- Get Direction to move forward or backward
	local len = VecLength(playerVec)
	if (len < minDist-0.5) then
		direction = VecNormalize(direction + VecNormalize(vec3.new(-playerVec.x,-playerVec.y,-playerVec.z)))
	end
	if (len > minDist+0.5) then
		direction = VecNormalize(direction + VecNormalize(playerVec))
	end

	-- Move the player
	
	body.velocity = direction * velocity
	--body.position = body.position + direction * velocity
	--transform.position = body.position
end

function Update()
	MoveInCirle()
end

Update()