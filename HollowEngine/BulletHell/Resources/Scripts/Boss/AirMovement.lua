function FollowPlayer()
    -----------------------------------------
    -- playtesting vars
	local enemySpeed = 2.0
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

function Update()
    local maxHealth = 200; 
	local health = gameObject:GetHealth()
    local hitPoints = health.hitPoints
    if (hitPoints < maxHealth / 3) then
		-- Third phase
    elseif (hitPoints < maxHealth * 2 / 3) then
		-- Second phase
    else
		-- First phase 
        FollowPlayer()
    end
end

Update()