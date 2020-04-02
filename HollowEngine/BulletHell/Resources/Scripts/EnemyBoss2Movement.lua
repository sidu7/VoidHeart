function LookForward()
    -----------------------------------------
    -- playtesting vars
	local enemySpeed = 20.0
	local scareSpeed = 8.0
    local scareDistance = 10 -- distance at which enemy will run away
    -----------------------------------------
    local transform = gameObject:GetTransform()
	local position = transform.position   
	-- Get forward position
	local targetPosX = position.x + transform:forward().x
	local targetPosZ = position.z + transform:forward().z

    -- calculate direction
    local xDir = targetPosX - position.x
	local zDir = targetPosZ - position.z
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
end

function RunAtThePlayer()
    -----------------------------------------
    -- playtesting vars
	local enemySpeed = 20.0
	local scareSpeed = 8.0
    local scareDistance = 10 -- distance at which enemy will run away
    -----------------------------------------
    local transform = gameObject:GetTransform()
	local position = transform.position   
	-- Get Player
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
    
    -- setting the velocity
    local body = gameObject:GetBody()
    body.velocity = vec3.new(xDirNorm, 0.0, zDirNorm)
	body.velocity = body.velocity * enemySpeed
    
    -- look at the target
    local rot = vec3.new(0.0, 0.0, 0.0)
    local tangent = body.velocity.x / body.velocity.z
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
end

function MeleeAttack()
    -- collision handling
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

    local attack = gameObject:GetAttack()
    if (dirLength < 3 and attack.shouldAttack == true) then
        local impulseStrength = vec3.new(1000, 0, 1000)
        local impulse = vec3.new(xDirNorm, 0.0, zDirNorm)
        impulse  = impulse * impulseStrength
        ApplyLinearImpulse(player, impulse)

        local impulseReverse = vec3.new(-xDirNorm, 0.0, -zDirNorm)
        impulseReverse = impulseReverse * impulseStrength
        ApplyLinearImpulse(gameObject, impulseReverse)
            
        local playerHealth = player:GetHealth()
        playerHealth.hitPoints = playerHealth.hitPoints - 1;
        attack.shouldAttack = false
    end
end

function LookAtThePlayer()
	local transform = gameObject:GetTransform()
	local selfPos = transform.position
	-- Get Player
	local target = player
    if (target == nil or target.isActive == false) then
        return
    end
    local targetTransform = target:GetTransform()
    local targetPos = targetTransform.position

    -- calculate direction
    local xDir = targetPos.x - selfPos.x
	local zDir = targetPos.z - selfPos.z
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
end

function Charge(currentTime, startTime, endTime)
    local material = gameObject:GetMaterial()
    local redChanel = 5*(currentTime - startTime) / (endTime - startTime)
    material.diffuse = vec3.new(redChanel,0,0)
end

function Cooldown(currentTime, startTime, endTime)
    local material = gameObject:GetMaterial()
    local redChanel = 1 - (currentTime - startTime) / (endTime - startTime)
    material.diffuse = vec3.new(redChanel,0,0)
end

function MoveToCenter()
  	local enemySpeed = 5.0
    
    -- collision handling
    local transform = gameObject:GetTransform()
	local position = transform.position
    --  acquire target 
	local room = GetDungeonFloor(currentFloor):GetRoomFromIndex(currentRoom)
	local roomCoords = room:GetCoords()
    local centerX = (roomCoords.y + roomCoords.y + 1) * roomSize / 2 
    local centerZ = (roomCoords.x + roomCoords.x + 1) * roomSize / 2 
    local targetPos = vec3.new(centerX, position.y, centerZ) 
    -- calculate direction
    local xDir = targetPos.x - position.x
	local zDir = targetPos.z - position.z
	local dirLength = math.sqrt(xDir*xDir + zDir*zDir)
	local xDirNorm = xDir / dirLength
	local zDirNorm = zDir / dirLength

    if (dirLength > 0.1) then -- walking to the center
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
    
        -- setting the velocity
        local body = gameObject:GetBody()
        body.velocity = vec3.new(xDirNorm, 0.0, zDirNorm)
	    if (isScared == true) then 
            if (dirLength < scareDistance) then
                body.velocity.x = scareDistance * body.velocity.x * -1
                body.velocity.z = scareDistance * body.velocity.z * -1
            else
                body.velocity = vec3.new(0.0, 0.0, 0.0)
            end    
        else
            body.velocity = body.velocity * enemySpeed
        end
    else -- look at the player
        local body = gameObject:GetBody()
        body.velocity = vec3.new(0.0, 0.0, 0.0)
        LookAtThePlayer()

        -- once at the center signal to attacking script to spawn rocks
        local attack = gameObject:GetAttack()
        if (attack.isFired2 ~= true) then
            attack.shouldAttack2 = true
        end
    end
end

function SteamParticles()
	local particle = gameObject:GetParticleEmitter()
	if particle.active ~= true then
	    particle.active = true
	    ChangeParticleShader(gameObject,"Resources/Shaders/SteamingModelParticles.comp")
	    particle.lifeRange = vec2.new(0.01,0.03)
	    particle.count = 0
	    particle.maxCount = 10000
	    particle.minColor = vec3.new( 0.5, 0.27, 0.07 )
	    particle.maxColor = vec3.new( 1.0, 1.0, 0.0 )
	    particle.scaleRange = vec2.new(0.01, 0.05)
	    particle.speedRange = vec2.new(1.0,2.0)
	    particle.fadeRate = 5.0
        particle.drawCount = 0
	end
end

function TrailingParticles()
	local particle = gameObject:GetParticleEmitter()
	if particle.active ~= true then
		particle.active = true
		ChangeParticleShader(gameObject,"Resources/Shaders/TrailingModelParticles.comp")
		particle.scaleRange = vec2.new(0.05, 0.1)
		particle.speedRange = vec2.new(1.0,2.0)
		particle.lifeRange = vec2.new(1.0,1.0)
		particle.minColor = vec3.new( 1.17, 0.08, 0.3 )
		particle.maxColor = vec3.new( 0.45, 0.24, 0.77)
		particle.fadeRate = 5.0
		particle.count = 0
		particle.maxCount = 10000
        particle.drawCount = 0
	end
	local transform = gameObject:GetTransform()
    local rot = transform.rotation
    local angle = rot.y
    angle = angle * math.pi / 180
    local xDir = math.sin(angle)
    local zDir = math.cos(angle)
	particle.direction = VecNormalize(vec3.new(-xDir,0,-zDir));
end

function ChangeParticles()
	local particle = gameObject:GetParticleEmitter()
	if particle then
		particle.extraData.x = 2.5
	end
	if particle.active ~= true then
	    particle.active = true
	    ChangeParticleShader(gameObject,"Resources/Shaders/ChargingModelParticles.comp")
	    particle.lifeRange = vec2.new(0.1,0.3)
	    particle.count = 0
	    particle.maxCount = 10000
	    particle.minColor = vec3.new( 0.5, 0.27, 0.07 )
	    particle.maxColor = vec3.new( 1.0, 0.0, 0.0 )
	    particle.scaleRange = vec2.new(0.01, 0.05)
	    particle.speedRange = vec2.new(1.0,2.0)
	    particle.fadeRate = 5.0
        particle.drawCount = 0
	end
end

function Update()
    local maxHealth = 200; 
	local health = gameObject:GetHealth()
    local hitPoints = health.hitPoints
    if (hitPoints < maxHealth / 3) then
        MoveToCenter()
    elseif (hitPoints < maxHealth * 2 / 3) then
        local particle = gameObject:GetParticleEmitter()
        if particle then
		    particle.active = false
	    end
        local material = gameObject:GetMaterial()
        material.diffuse = vec3.new(0,0,0)
        MoveToCenter()
    else
        local attack = gameObject:GetAttack()
        attack.baseAttackTime = 9
	    if (attack.currentAttackTime > attack.baseAttackTime) then
            attack.currentAttackTime = 0
            local particle = gameObject:GetParticleEmitter()
            if particle then
		        particle.active = false
	        end
        elseif (attack.currentAttackTime > attack.baseAttackTime * 2 / 3) then
            Cooldown(attack.currentAttackTime, attack.baseAttackTime * 2 /3, attack.baseAttackTime)
            if (attack.IsFired == true) then
                local particle = gameObject:GetParticleEmitter()
                if particle then
		            particle.active = false
	            end
            end
            attack.IsFired = false -- resetting after the RunAtThePlayer
            attack.shouldAttack = true -- resets for MeleeAttack
            LookForward()
            SteamParticles()
        elseif (attack.currentAttackTime > attack.baseAttackTime / 3) then
            if (attack.IsFired == false) then
                local particle = gameObject:GetParticleEmitter()
	            if particle then
		            particle.active = false
	            end
                RunAtThePlayer()
                attack.IsFired = true
                PlaySFX("Resources/Audio/SFX/BossChargingAttack.wav")
            else
                LookForward()
                TrailingParticles()
            end

            MeleeAttack()
        else
            LookAtThePlayer()
	        local body = gameObject:GetBody()
            body.velocity = vec3.new(0.0, 0.0, 0.0)
            Charge(attack.currentAttackTime, 0, attack.baseAttackTime / 3)
            ChangeParticles()
            --PlaySFX("Resources/Audio/SFX/BossCharging.wav")
        end
        -- Handled By EnemyBoss2Fire.lua, considering cooldown for each charge
    end

    local body = gameObject:GetBody()
    body.angularVelocity = vec3.new(0,0,0)
end

Update()