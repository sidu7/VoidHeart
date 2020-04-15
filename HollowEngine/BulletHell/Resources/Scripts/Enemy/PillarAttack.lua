function Shoot()
    -----------------------------------------
    -- playtesting vars
	local bulletSpeed = 6.0
    -----------------------------------------
	 local body = gameObject:GetBody()    
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
    
	local attack = gameObject:GetAttack()
	if attack.currentAttackTime > attack.baseAttackTime then
        -- Create bullet
	    local bulletPrefabPath = "Resources/Json data/Bullet.json"
	    local bullet = CreateGameObject(bulletPrefabPath)
        local bulletTransform = bullet:GetTransform()
	    local bulletBody = bullet:GetBody()
   
        -- Setting position
        bulletBody.position = spawnPos + 0.7 * transform:forward()
		bulletBody.position.y = 2.6
        bulletTransform.position = spawnPos

        -- Setting velocity
        bulletBody.velocity = bulletSpeed * vec3.new(xDirNorm, 0.0, zDirNorm)
    
        -- Setting rotation
        bulletBody:RotateBody(rot);

        attack.currentAttackTime = 0
		PlaySFX("Resources/Audio/SFX/EnemyAttackShootAtPlayer.wav")
    end

end

function Update()
    
	Shoot()
end

Update()