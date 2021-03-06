function PhaseOneBehavior()
	local spawnPos = gameObject:GetTransform().position

	local bulletPrefabPath = "Resources/Json data/BossWaterBullet.json"
	local bullet = CreateGameObject(bulletPrefabPath)
    local bulletTransform = bullet:GetTransform()
	local bulletBody = bullet:GetBody()
    
    -- Setting position
    bulletBody.position = spawnPos
    bulletTransform.position = spawnPos
	PlaySFX("Resources/Audio/SFX/BossWaterBullet.wav")
end

function CreateSteamTrap()
	-----------------------------------------
    -- playtesting vars
	
    -----------------------------------------
	local trapPath = "Resources/Json data/BossSteamTrap.json"
	local trap = CreateGameObject(trapPath)
    local trapTransform = trap:GetTransform()
	local trapBody = trap:GetBody()
	
	trapBody.position = gameObject:GetTransform().position
	trapTransform.position = trapBody.position

	local particle = trap:GetParticleEmitter()
	if particle then
		particle.extraData.x = trapTransform.scale.x;
	end
	PlaySFX("Resources/Audio/SFX/BossWaterSteamTrap.wav")
end

function PhaseTwoBehavior()
-----------------------------------------
    -- playtesting vars
	
	-----------------------------------------
	CreateSteamTrap()
    
end

function PhaseThreeBehavior()
	-----------------------------------------
    -- playtesting vars
	local attack1CycleTime = 1
	local attack2CycleTime = 12
    -----------------------------------------
	local attack = gameObject:GetAttack()
	attack.baseAttackTime = attack1CycleTime

	PhaseOneBehavior()
	waterBossAttackTime = waterBossAttackTime + attack1CycleTime

	if(waterBossAttackTime == attack2CycleTime) then
		PhaseTwoBehavior()
		waterBossAttackTime = 0
	end
end

function Shoot()
	-----------------------------------------
    -- playtesting vars
	local maxHealth = 150
    -----------------------------------------
	
    local health = gameObject:GetHealth()
    local hitPoints = health.hitPoints
	
	local attack = gameObject:GetAttack()
	if(hitPoints >= maxHealth * 2.0 / 3.0) then
		-- First phase
		PhaseOneBehavior()
		attack.baseAttackTime = 0.5
	elseif(hitPoints >= maxHealth / 3.0) then
		-- Second phase
		if(attack.shouldAttack2 ==  true) then
			PhaseTwoBehavior()
		end
		attack.baseAttackTime = 4
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