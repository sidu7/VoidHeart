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
	local phaseOneDashTime = 3.0
	local phaseOneAOETime = 0.5
    -----------------------------------------
	local go = CreatePrefabAtPosition("SteamTrap", gameObject:GetTransform().position)
	local body = go:GetBody()
	
	transform = go:GetTransform()
	transform.position = body.position

	local particle = go:GetParticleEmitter()
	if particle then
		particle.extraData.x = transform.scale.x;
	end
end

function PhaseTwoBehavior()
-----------------------------------------
    -- playtesting vars
	local shieldDelayTime = 2.5
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
		PhaseTwoBehavior()
		attack.baseAttackTime = 3
	else
		-- Third phase
		PhaseThreeBehavior()
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