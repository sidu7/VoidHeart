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
	PlaySFX("Resources/Audio/SFX/BossAirAOE.wav")
end

function SetPillarNearestPlayer()
	-----------------------------------------
    -- playtesting vars
	local offset = vec3.new(0.0, 2.0, 0.0)
	-----------------------------------------
	-- Get the current room/pillars
	local floor = GetDungeonFloor(currentFloor)
	local currentRoomObj = floor:GetRoomFromIndex(currentRoom)
	local pillars = currentRoomObj.obstacles
	local playerBody = player:GetBody()
	
	-- Find pillar closest to the player
	local closestPillarIndex = 0
	local closestPillarDistance = 100000000.0
	for k=1,#pillars do
		local pillarDistance = VecLength(playerBody.position - pillars[k]:GetTransform().position)
		if pillarDistance < closestPillarDistance then
			closestPillarIndex = k
			closestPillarDistance = pillarDistance
		end
	end
	closestPillar = pillars[closestPillarIndex]
end

function SetPillarFurthestPlayer()
-----------------------------------------
    -- playtesting vars
	local offset = vec3.new(0.0, 2.0, 0.0)
	-----------------------------------------
	-- Get the current room/pillars
	local floor = GetDungeonFloor(currentFloor)
	local currentRoomObj = floor:GetRoomFromIndex(currentRoom)
	local pillars = currentRoomObj.obstacles
	local playerBody = player:GetBody()
	
	-- Find pillar closest to the player
	local furthestPillarIndex = 0
	local furthestPillarDistance = -1.0
	for k=1,#pillars do
		local pillarDistance = VecLength(playerBody.position - pillars[k]:GetTransform().position)
		if pillarDistance > furthestPillarDistance then
			furthestPillarIndex = k
			furthestPillarDistance = pillarDistance
		end
	end
	closestPillar = pillars[furthestPillarIndex]
end

function JumpInAir()
	local impulse = vec3.new(0.0, 16000.0, 0.0)
	ApplyLinearImpulse(gameObject, impulse)
end

function PhaseOneBehavior()
	-----------------------------------------
    -- playtesting vars
	local phaseOneDashTime = 3.0
	local phaseOneAOETime = 0.5
    -----------------------------------------
	local attack = gameObject:GetAttack()
	attack.shouldAttack = attack.currentAttackTime > attack.baseAttackTime
	if attack.shouldAttack then
		if dashFlag then
			AreaDamage()
			attack.baseAttackTime = phaseOneDashTime
		else
			DashAtPlayer()
			attack.baseAttackTime = phaseOneAOETime
		end
		attack.currentAttackTime = 0.0
	end
end

function PhaseTwoBehavior()
-----------------------------------------
    -- playtesting vars
	local minDistanceToPillar = 2.5
	local jumpDelayTime = 2.5
	-----------------------------------------
	local attack = gameObject:GetAttack()
	local position = gameObject:GetBody().position
	local distance = VecLength(centerPillar:GetBody().position - position)
	-- Once boss is close enough to center pillar start countdown to jump
	if distance < minDistanceToPillar and not dashFlag then
		attack.currentAttackTime = 0.0
		attack.baseAttackTime = jumpDelayTime
		dashFlag = true
	end
	-- After countdown is reached, find pillar to jump to
	if dashFlag and attack.currentAttackTime > attack.baseAttackTime then
		SetPillarNearestPlayer()
		JumpInAir()
		attack.shouldAttack = false
		attack.baseAttackTime = 1000.0
		dashFlag = false
	end
	
	-- Once close enough to the pillar to jump to activate attack
	if closestPillar ~= nil then 
		
		local distanceToPillar = VecLength(closestPillar:GetBody().position - position)
		if distanceToPillar < minDistanceToPillar then
			-- Activate actual attack
			AreaDamage()
			closestPillar = nil
		end
	end
    
end

function PhaseThreeBehavior()
	-----------------------------------------
    -- playtesting vars
	local totalAttackCycleTime = 64.0
	local dashTime = 6.0
	local AOETime = dashTime + 0.5
	local getPillarTime = totalAttackCycleTime / 2.0
	local moveToPillarTime = getPillarTime + 0.5
	
	local minDistanceToPillar = 2.5
    -----------------------------------------
	local attack = gameObject:GetAttack()
	attack.baseAttackTime = totalAttackCycleTime
	
	if attack.currentAttackTime > dashTime and attack.currentAttackTime < AOETime then
		-- Dash at player, dont reset or set anything
		if not dashFlag then
			DashAtPlayer()
		end
	elseif attack.currentAttackTime > AOETime and
	attack.currentAttackTime < getPillarTime then
		-- Cast AOE, set timer to next cycle to get pillar
		if dashFlag then
			AreaDamage()
			attack.currentAttackTime = getPillarTime
			dashFlag = false
		end
	elseif attack.currentAttackTime > getPillarTime and
	attack.currentAttackTime < moveToPillarTime then
		-- Get pillar to move to
		SetPillarFurthestPlayer()
	elseif attack.currentAttackTime > moveToPillarTime then
		-- Check if close enough to pillar to give life and transition back into dash attack, reset pillar and current attack timer
		local position = gameObject:GetBody().position
		if closestPillar == nil then
			SetPillarFurthestPlayer()
		end
		local pillarPosition = closestPillar:GetBody().position
		local distance = VecLength(pillarPosition - position)
		if(distance < minDistanceToPillar) then
			closestPillar:GetAttack().shouldAttack = true
			closestPillar:GetAttack().isActive = true
			closestPillar = nil
			attack.currentAttackTime = 0.0
			PlaySFX("Resources/Audio/SFX/BossAirActivatePillar.wav")
		end
	end	
end

function Update()
	-----------------------------------------
    -- playtesting vars
	local maxHealth = 180
    -----------------------------------------
	--print(gameObject.id)
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

Update()