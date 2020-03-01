function ShootInDirection(dirX, dirY, dirZ)
    -----------------------------------------
    -- playtesting vars
	local bulletSpeed = 10.0
    -----------------------------------------
    local transform = gameObject:GetTransform()
	local spawnPos = transform.position

	local bulletPrefabPath = "Resources/Json data/Bullet.json"
	local bullet = CreateGameObject(bulletPrefabPath)
    local bulletTransform = bullet:GetTransform()
	local bulletBody = bullet:GetBody()
    
    -- Setting position
    bulletBody.position = spawnPos
    bulletTransform.position = spawnPos

    -- Setting velocity
    local direction = vec3.new(dirX, dirY, dirZ)
    local length = dirX * dirX + dirY * dirY + dirZ * dirZ
    direction = direction * (1 / length);
    bulletBody.velocity = bulletSpeed * direction
    bulletBody.useGravity = true
end

function CreateLargeFireball()
	local bulletSpeed = 25.0
    
    --  acquire target 
    local target = player
    if (target == nil or target.isActive == false) then
        return
    end
    local targetTransform = target:GetTransform()
    local targetPos = targetTransform.position
    
    local transform = gameObject:GetTransform()
	local spawnPos = transform.position


    -- Create bullet
	local bullet = CreatePrefabAtPosition("EnemyLargeFireball", gameObject:GetTransform().position)
    local bulletTransform = bullet:GetTransform()
    local bulletBody = bullet:GetBody()
	
    -- Setting position
    bulletBody.position = spawnPos
    bulletTransform.position = spawnPos
		
    -- calculate direction/velocity
    local xDir = targetPos.x - spawnPos.x
	local zDir = targetPos.z - spawnPos.z
	local dirLength = math.sqrt(xDir*xDir + zDir*zDir)
	local xDirNorm = xDir / dirLength
	local zDirNorm = zDir / dirLength
    bulletBody.velocity = bulletSpeed * vec3.new(xDirNorm, 0.0, zDirNorm)

	-- Change Color
	material = bullet:GetMaterial()
	material.diffuse = vec3.new(4.0, 0.0, 0.0)
end

function ShootInAllDirections()
    
	local offset = 0
    local numBullets = 20
    for i = 0, numBullets do
	    local theta = (i / numBullets * math.pi * 2) + math.rad(offset)
        ShootInDirection(math.cos(theta), 0.5, math.sin(theta))
    end
end

function FlameThrower()
    --  acquire target 
    local target = player
    if (target == nil or target.isActive == false) then
        return
    end
    local targetTransform = target:GetTransform()
    local targetPos = targetTransform.position
    local transform = gameObject:GetTransform()
	local spawnPos = transform.position
	local go = CreatePrefabAtPosition("PlayerFlames", spawnPos)
    go.type = 5
	local body = go:GetBody()

    -- calculate direction
    local xDir = targetPos.x - spawnPos.x
	local zDir = targetPos.z - spawnPos.z
	local dirLength = math.sqrt(xDir*xDir + zDir*zDir)
	local xDirNorm = xDir / dirLength
	local zDirNorm = zDir / dirLength

	local attackSpeed = 5.0
	local enemyVelocity = gameObject:GetBody().velocity
	body.velocity = enemyVelocity + attackSpeed * vec3.new(xDirNorm, 0.0, zDirNorm)
	-- Add player velocity

	transform = go:GetTransform()
	transform.position = body.position

	-- Change Color
	material = go:GetMaterial()
	material.diffuse = vec3.new(4.0, 0.0, 0.0)
end

function Shoot()
    local health = gameObject:GetHealth()
	local attack = gameObject:GetAttack()
    local hitPoints = health.hitPoints
    if (hitPoints < 33) then
        CreateLargeFireball()
        attack.baseAttackTime = 3
    elseif (hitPoints < 66) then
        ShootInAllDirections()
        attack.baseAttackTime = 1.5
    else
        FlameThrower()
        attack.baseAttackTime = 0.1
    end
end

function Update()
	local attack = gameObject:GetAttack()
	if attack.currentAttackTime > attack.baseAttackTime then
	    Shoot()
        attack.currentAttackTime = 0
    end
end

Update()