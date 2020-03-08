function DashAtPlayer()
	-----------------------------------------
    -- playtesting vars
	local dashSpeed = 4000000.0
    -----------------------------------------
	
	local body = gameObject:GetBody()
	local transform = gameObject:GetTransform()
    local rot = transform.rotation
    local angle = rot.y
    angle = angle * math.pi / 180
    local xDir = math.sin(angle)
    local zDir = math.cos(angle)

	local direction = vec3.new(xDir, 0, zDir)--body.velocity
	local magnitude = math.sqrt(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z)
	if magnitude > 0.001 then
		direction.x = direction.x / magnitude
		direction.y = direction.y / magnitude
		direction.z = direction.z / magnitude
		local impulse = vec3.new(0.0, 0.0, 0.0)
		impulse = impulse + direction * dashSpeed
		impulse.y = 0.0
		PlaySFX("Resources/Audio/SFX/PlayerDash.wav")
		ApplyLinearImpulse(gameObject, impulse)
	end
	
	dashFlag = true
end

function AreaDamage()
	local transform = gameObject:GetTransform()
	local spawnPos = transform.position

	local AOEPrefabPath = "Resources/Json data/EnemyAOE.json"
	local AOE = CreateGameObject(AOEPrefabPath)
    local AOETransform = AOE:GetTransform()
	local AOEBody = AOE:GetBody()
    
    -- Setting position
    AOEBody.position = spawnPos
    AOETransform.position = spawnPos
	dashFlag = false
end

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

	transform = go:GetTransform()
	transform.position = body.position

	-- Change Color
	material = go:GetMaterial()
	material.diffuse = vec3.new(4.0, 0.0, 0.0)
end

function PhaseOneBehavior()
	-----------------------------------------
    -- playtesting vars
	local phaseOneDashTime = 3.0
	local phaseOneAOETime = 0.5
    -----------------------------------------
	local attack = gameObject:GetAttack()
	
	if dashFlag then
		AreaDamage()
		attack.baseAttackTime = phaseOneDashTime
	else
		DashAtPlayer()
		attack.baseAttackTime = phaseOneAOETime
	end
end

function PhaseTwoBehavior()
	local attack = gameObject:GetAttack()
	ShootInAllDirections()
    attack.baseAttackTime = 3
end

function PhaseThreeBehavior()
	local attack = gameObject:GetAttack()
end

function Shoot()
	-----------------------------------------
    -- playtesting vars
	local maxHealth = 200
    -----------------------------------------
	
    local health = gameObject:GetHealth()
    local hitPoints = health.hitPoints
	
	if(hitPoints >= maxHealth * 2.0 / 3.0) then
		-- First phase
		PhaseOneBehavior()
	elseif(hitPoints >= maxHealth / 3.0) then
		-- Second phase
		PhaseTwoBehavior()
	else
		-- Third phase
		PhaseThreeBehavior()
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