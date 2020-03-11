function Shoot()
    -----------------------------------------
    -- playtesting vars
	local bulletSpeed = 15.0
    -----------------------------------------
	   
    local transform = gameObject:GetTransform()
	local spawnPos = transform.position

    --  acquire target 
    local target = player
    if (target == nil or target.isActive == false) then
        return
    end
    local targetTransform = target:GetTransform()
    local targetPos = targetTransform.position
    
    -- calculate direction
    local xDir = targetPos.x - spawnPos.x
	local zDir = targetPos.z - spawnPos.z
	local dirLength = math.sqrt(xDir*xDir + zDir*zDir)
	local xDirNorm = xDir / dirLength
	local zDirNorm = zDir / dirLength
	
    -- look at the target
    local tangent = xDirNorm / zDirNorm
    local radians = math.atan(tangent)
    local degree = radians * 180 / math.pi
	local rot = vec3.new(0.0, degree, 0.0)
    if zDirNorm < 0 then  
	    rot = rot + vec3.new(0.0, 180.0, 0.0)
    end
    transform:Rotate(rot)
	
	local attack = gameObject:GetAttack()
	if attack.currentAttackTime > attack.baseAttackTime then
        -- Create bullet
	    local bulletPrefabPath = "Resources/Json data/Bullet.json"
	    local bullet = CreateGameObject(bulletPrefabPath)
        local bulletTransform = bullet:GetTransform()
	    local bulletBody = bullet:GetBody()
   
        -- Setting position
        bulletBody.position = spawnPos + vec3.new(0.0, 1.0, 0.0)
        bulletTransform.position = bulletBody.position

        -- Setting velocity
        bulletBody.velocity = bulletSpeed * vec3.new(xDirNorm, 0.0, zDirNorm)

        attack.currentAttackTime = 0
    end
end

function Update()
    local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		Shoot()
	end
end

Update()