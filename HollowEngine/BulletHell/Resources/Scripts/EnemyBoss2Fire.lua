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
    local distanceFromBoss = 3
    -----------------------------------------
    if (player == nil or player.isActive == false) then
        return
    end
    local playerTransform = player:GetTransform()
    local playerPos = playerTransform.position
    
    local transform = gameObject:GetTransform()
	local spawnPos = vec3.new(transform.position.x, transform.position.y, transform.position.z)
    spawnPos.y = spawnPos.y 
	local rockPrefabPath = "Resources/Json data/EnemyBeam.json"
	local rock = CreateGameObject(rockPrefabPath)
    local rockTransform = rock:GetTransform()
	local rockBody = rock:GetBody()
    
    -- Setting position
    local direction = vec3.new(dirX, dirY, dirZ)
    local length = dirX * dirX + dirY * dirY + dirZ * dirZ
    direction = direction * (1 / length);
    rockBody.position = spawnPos + direction * distanceFromBoss
    rockTransform.position = rockBody.position

end

function SpawnLasers()
	local offset = math.pi / 3
    local numLasers = 3
    for i = 1, numLasers do
	    local theta = (i / numLasers * math.pi * 2) + math.rad(offset)
        SpawnLaser(math.cos(theta), 0.5, math.sin(theta))
    end
end

function MeteorShower()
	for i=0, 10, 1 do
		local go = CreatePrefabAtPosition("EnemyMeteor", gameObject:GetTransform().position)
		local body = go:GetBody()
		
		local transform = gameObject:GetTransform()
		local rot = transform.rotation
		local angle = rot.y
		angle = angle * math.pi / 180
		local xDir = math.sin(angle)
		local zDir = math.cos(angle)

		local xOffset = math.random(-5, 5)
		local zOffset = math.random(-5, 5)

		body.position = transform.position + vec3.new(6.0 * xDir + xOffset, 15.0 + 5.0 * i, 6.0 * zDir + zOffset)
	
		local attackSpeed = 20.0
		body.velocity = attackSpeed * vec3.new(0.0, -1.0, 0.0)

		transform = go:GetTransform()
		transform.position = body.position
	end
end

function Update()
    local maxHealth = 200
    local health = gameObject:GetHealth()
	local attack = gameObject:GetAttack()
    local hitPoints = health.hitPoints
	local attack = gameObject:GetAttack()
    if (hitPoints < maxHealth / 3) then
        attack.baseAttackTime = 1.5
	    if attack.currentAttackTime > attack.baseAttackTime then
            MeteorShower()
            --CreateLargeFireball()
            attack.currentAttackTime = 0
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