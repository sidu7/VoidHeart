function MoveInCirle()
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

	-- Get Direction to move in circle
	local playerVec = targetPos - selfPos
	local Y = vec3.new(0,1,0)
	local direction = VecNormalize(VecCross(Y,playerVec));
	-- if enemy collides with wall reverse direction VecCross(Y,playerVec)
	-- need event for WALL ENEMY collision
	
	-- Get Direction to move forward or backward
	local minDist = 6
	local len = VecLength(playerVec)
	if (len < minDist-0.5) then
		direction = VecNormalize(direction + VecNormalize(vec3.new(-playerVec.x,-playerVec.y,-playerVec.z)))
	end
	if (len > minDist+0.5) then
		direction = VecNormalize(direction + VecNormalize(playerVec))
	end

	-- Move the player
	local velocity = 5
	body.velocity = direction * velocity
	--body.position = body.position + direction * velocity
	--transform.position = body.position
end

function Update()
	MoveInCirle()
end

Update()