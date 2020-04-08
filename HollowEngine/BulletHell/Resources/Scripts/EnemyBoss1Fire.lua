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
	local bulletSpeed = 15.0
    
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
	local spawnPos = vec3.new(transform.position.x, transform.position.y, transform.position.z)
    spawnPos.y = spawnPos.y + transform.scale.y * 0.30
    local forward = vec3. new(transform:forward().x, transform:forward().y, transform:forward().z)
    forward = VecNormalize(forward)
	spawnPos = spawnPos + forward

	local go = CreatePrefabAtPosition("PlayerFlames", spawnPos)
    go.type = 5
	local body = go:GetBody()

    -- calculate direction
    local xDir = targetPos.x - spawnPos.x
    local yDir = targetPos.y - spawnPos.y
	local zDir = targetPos.z - spawnPos.z
	local dirLength = math.sqrt(xDir*xDir + yDir * yDir + zDir*zDir)
	local xDirNorm = xDir / dirLength
    local yDirNorm = yDir / dirLength
	local zDirNorm = zDir / dirLength


	local attackSpeed = 5.0
	local enemyVelocity = gameObject:GetBody().velocity
	body.velocity = enemyVelocity + attackSpeed * vec3.new(xDirNorm, yDirNorm, zDirNorm)
	-- Add player velocity

    
	local particle = gameObject:GetParticleEmitter()
	if particle.active ~= true then
		particle.active = true
		particle.count = 0
		ChangeParticleShader(gameObject,"Resources/Shaders/DirectionalConeParticles.comp")
		particle.extraData.x = 20.0
		particle.scaleRange = vec2.new(0.2, 0.2)
		particle.speedRange = vec2.new(4.0,5.0)
		particle.lifeRange = vec2.new(0.2,0.5)
		particle.minColor = vec3.new(1.0, 1.0, 0.0)
		particle.maxColor = vec3.new(1.0, 0.0, 0.0)
		particle.maxCount = 1000
	end
	particle.direction = VecNormalize(vec3.new(xDir,0,zDir));


	transform = go:GetTransform()
	transform.position = body.position

	-- Change Color
	material = go:GetMaterial()
	material.diffuse = vec3.new(4.0, 0.0, 0.0)
end

function Shoot()
    local maxHealth = 200
    local health = gameObject:GetHealth()
	local attack = gameObject:GetAttack()
    local hitPoints = health.hitPoints
    --print(hitPoints)
    if (hitPoints < maxHealth / 3) then
        CreateLargeFireball()
        attack.baseAttackTime = 3
    elseif (hitPoints < maxHealth * 2 / 3) then
        ShootInAllDirections()
        attack.baseAttackTime = 3
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