function FollowPlayer()
    -----------------------------------------
    -- playtesting vars
	local enemySpeed = 4.0
    -----------------------------------------

    local transform = gameObject:GetTransform()
	local position = transform.position

    --  acquire target 
    local target = player
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
        transform:Rotate(rot)
    end
    if zDirNorm < 0 then 
	    rot = vec3.new(0.0, degree + 180, 0.0)
        transform:Rotate(rot)
    end

    local body = gameObject:GetBody()
    body.velocity = enemySpeed * vec3.new(xDirNorm, 0.0, zDirNorm)
    
    -- collision handling
    if (dirLength < 1.0) then
        local impulseStrength = vec3.new(1000, 0, 1000)
        local impulse = vec3.new(xDirNorm, 0.0, zDirNorm)
        impulse  = impulse * impulseStrength
        ApplyLinearImpulse(player, impulse)

        local impulseReverse = vec3.new(-xDirNorm, 0.0, -zDirNorm)
        impulseReverse = impulseReverse * impulseStrength
        ApplyLinearImpulse(gameObject, impulseReverse)
    end
end

function Update()
    
	FollowPlayer()
end

Update()