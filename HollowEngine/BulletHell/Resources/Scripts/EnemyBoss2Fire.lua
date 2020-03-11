--function ShootInDirection(dirX, dirY, dirZ)
--    -----------------------------------------
--    -- playtesting vars
--	local bulletSpeed = 10.0
--    -----------------------------------------
--    local transform = gameObject:GetTransform()
--	local spawnPos = transform.position
--
--	local bulletPrefabPath = "Resources/Json data/Bullet.json"
--	local bullet = CreateGameObject(bulletPrefabPath)
--    local bulletTransform = bullet:GetTransform()
--	local bulletBody = bullet:GetBody()
--    
--    -- Setting position
--    bulletBody.position = spawnPos
--    bulletTransform.position = spawnPos
--
--    -- Setting velocity
--    local direction = vec3.new(dirX, dirY, dirZ)
--    local length = dirX * dirX + dirY * dirY + dirZ * dirZ
--    direction = direction * (1 / length);
--    bulletBody.velocity = bulletSpeed * direction
--    bulletBody.useGravity = true
--end
--
--function CreateLargeFireball()
--	local bulletSpeed = 25.0
--    
--    --  acquire target 
--    local target = player
--    if (target == nil or target.isActive == false) then
--        return
--    end
--    local targetTransform = target:GetTransform()
--    local targetPos = targetTransform.position
--    
--    local transform = gameObject:GetTransform()
--	local spawnPos = transform.position
--
--
--    -- Create bullet
--	local bullet = CreatePrefabAtPosition("EnemyLargeFireball", gameObject:GetTransform().position)
--    local bulletTransform = bullet:GetTransform()
--    local bulletBody = bullet:GetBody()
--	
--    -- Setting position
--    bulletBody.position = spawnPos
--    bulletTransform.position = spawnPos
--		
--    -- calculate direction/velocity
--    local xDir = targetPos.x - spawnPos.x
--	local zDir = targetPos.z - spawnPos.z
--	local dirLength = math.sqrt(xDir*xDir + zDir*zDir)
--	local xDirNorm = xDir / dirLength
--	local zDirNorm = zDir / dirLength
--    bulletBody.velocity = bulletSpeed * vec3.new(xDirNorm, 0.0, zDirNorm)
--
--	-- Change Color
--	material = bullet:GetMaterial()
--	material.diffuse = vec3.new(4.0, 0.0, 0.0)
--end
--
--function ShootInAllDirections()
--    
--	local offset = 0
--    local numBullets = 20
--    for i = 0, numBullets do
--	    local theta = (i / numBullets * math.pi * 2) + math.rad(offset)
--        ShootInDirection(math.cos(theta), 0.5, math.sin(theta))
--    end
--end
--
--function FlameThrower()
--    --  acquire target 
--    local target = player
--    if (target == nil or target.isActive == false) then
--        return
--    end
--    local targetTransform = target:GetTransform()
--    local targetPos = targetTransform.position
--    local transform = gameObject:GetTransform()
--	local spawnPos = vec3.new(transform.position.x, transform.position.y, transform.position.z)
--    spawnPos.y = spawnPos.y + transform.scale.y * 0.30
--    local forward = vec3. new(transform:forward().x, transform:forward().y, transform:forward().z)
--    forward = VecNormalize(forward)
--	spawnPos = spawnPos + forward
--
--	local go = CreatePrefabAtPosition("PlayerFlames", spawnPos)
--    go.type = 5
--	local body = go:GetBody()
--
--    -- calculate direction
--    local xDir = targetPos.x - spawnPos.x
--    local yDir = targetPos.y - spawnPos.y
--	local zDir = targetPos.z - spawnPos.z
--	local dirLength = math.sqrt(xDir*xDir + yDir * yDir + zDir*zDir)
--	local xDirNorm = xDir / dirLength
--    local yDirNorm = yDir / dirLength
--	local zDirNorm = zDir / dirLength
--
--
--	local attackSpeed = 5.0
--	local enemyVelocity = gameObject:GetBody().velocity
--	body.velocity = enemyVelocity + attackSpeed * vec3.new(xDirNorm, yDirNorm, zDirNorm)
--	-- Add player velocity
--
--	transform = go:GetTransform()
--	transform.position = body.position
--
--	-- Change Color
--	material = go:GetMaterial()
--	material.diffuse = vec3.new(4.0, 0.0, 0.0)
--end

function SpawnRock(dirX, dirY, dirZ)
    local distanceFromBoss = 2
    -----------------------------------------
    if (player == nil or player.isActive == false) then
        return
    end
    local playerTransform = player:GetTransform()
    local playerPos = playerTransform.position
    local room = GetDungeonFloor(currentFloor):GetRoomFromIndex(currentRoom)
	local roomCoords = room:GetCoords()
    local centerX = (roomCoords.y + roomCoords.y + 1) * roomSize / 2 
    local centerZ = (roomCoords.x + roomCoords.x + 1) * roomSize / 2 
    local centerPos = vec3.new(centerX, playerPos.y, centerZ) 
    

    local transform = gameObject:GetTransform()
	local spawnPos = centerPos
    
	local rockPrefabPath = "Resources/Json data/CirclingBullet.json"
	local rock = CreateGameObject(rockPrefabPath)
    local rockTransform = rock:GetTransform()
	local rockBody = rock:GetBody()
    
    -- Setting position
    local direction = vec3.new(dirX, dirY, dirZ)
    local length = dirX * dirX + dirY * dirY + dirZ * dirZ
    direction = direction * (1 / length);
    rockBody.position = spawnPos + direction * distanceFromBoss
    rockTransform.position = spawnPos + direction * distanceFromBoss

end

function SpawnRocks()
	local offset = 0
    local numRocks = 3
    for i = 0, numRocks do
	    local theta = (i / numRocks * math.pi * 2) + math.rad(offset)
        SpawnRock(math.cos(theta), 0.5, math.sin(theta))
    end
end

function SpawnLaser(dirX, dirY, dirZ)
    local distanceFromBoss = 2
    -----------------------------------------
    if (player == nil or player.isActive == false) then
        return
    end
    local playerTransform = player:GetTransform()
    local playerPos = playerTransform.position
    
    local transform = gameObject:GetTransform()
	local spawnPos = transform.position
    
	local rockPrefabPath = "Resources/Json data/EnemyBeam.json"
	local rock = CreateGameObject(rockPrefabPath)
    local rockTransform = rock:GetTransform()
	local rockBody = rock:GetBody()
    
    -- Setting position
    local direction = vec3.new(dirX, dirY, dirZ)
    local length = dirX * dirX + dirY * dirY + dirZ * dirZ
    direction = direction * (1 / length);
    rockBody.position = spawnPos + direction * distanceFromBoss
    rockTransform.position = spawnPos + direction * distanceFromBoss

end

function SpawnLasers()
	local offset = math.pi / 3
    local numRocks = 3
    for i = 0, numRocks do
	    local theta = (i / numRocks * math.pi * 2) + math.rad(offset)
        SpawnLaser(math.cos(theta), 0.5, math.sin(theta))
    end
end

function Update()
    local maxHealth = 200
    local health = gameObject:GetHealth()
	local attack = gameObject:GetAttack()
    local hitPoints = health.hitPoints
	local attack = gameObject:GetAttack()
    
    if (hitPoints < maxHealth / 3) then
	    if attack.currentAttackTime > attack.baseAttackTime then
            --CreateLargeFireball()
        --    attack.currentAttackTime = 0
        end
        --attack.baseAttackTime = 3
    elseif (hitPoints < maxHealth * 2 / 3) then
        
        attack.baseAttackTime = 10
        if (attack.shouldAttack2 == true and attack.IsFired2 == false) then
            SpawnRocks()
            attack.shouldAttack2 = false
            attack.IsFired2 = true
            
        end

        if (attack.currentAttackTime > attack.baseAttackTime) then
            attack.currentAttackTime = 0
        elseif (attack.currentAttackTime > attack.baseAttackTime / 2) then
            if (attack.shouldAttack == true) then
                SpawnLasers()
                attack.shouldAttack = false
            end
        else
            attack.shouldAttack = true
        end
    else 
        attack.shouldAttack2 = false
	    --melee handled by EnemyBoss2Movement.lua
    end
end

Update()