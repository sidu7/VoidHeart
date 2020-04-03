function PhaseOneBehavior()
	local spawnPos = gameObject:GetTransform().position

	local bulletPrefabPath = "Resources/Json data/BossWaterBullet.json"
	local bullet = CreateGameObject(bulletPrefabPath)
    local bulletTransform = bullet:GetTransform()
	local bulletBody = bullet:GetBody()
    
    -- Setting position
    bulletBody.position = spawnPos
    bulletTransform.position = spawnPos
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
	local totalAttackCycleTime = 64.0
	local dashTime = 6.0
	local AOETime = dashTime + 0.5
	local getPillarTime = totalAttackCycleTime / 2.0
	local moveToPillarTime = getPillarTime + 0.5
	
	local minDistanceToPillar = 2.5
    -----------------------------------------
	local attack = gameObject:GetAttack()
	attack.baseAttackTime = totalAttackCycleTime
	
	
end

function Shoot()
	-----------------------------------------
    -- playtesting vars
	local maxHealth = 200
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
		attack.baseAttackTime = 1
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