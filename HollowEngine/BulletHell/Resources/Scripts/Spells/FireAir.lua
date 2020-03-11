function CreateFireball(xVel, zVel)
	local go = CreatePrefabAtPosition("PlayerFlames", gameObject:GetTransform().position)
	local body = go:GetBody()
		
	local transform = gameObject:GetTransform()
    local rot = transform.rotation
    local angle = rot.y
    angle = angle * math.pi / 180
    local xDir = math.sin(angle)
    local zDir = math.cos(angle)

	local xVelocity = GetAxis(CONTROLLER["RX"])
	local zVelocity = GetAxis(CONTROLLER["RY"])
    if ((xVelocity < - 16000) or (xVelocity > 16000) or (zVelocity < -16000) or (zVelocity > 16000)) then 
        local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
        xVelocity = xVelocity / totalVelocity
        zVelocity = zVelocity / totalVelocity
        xDir = xVelocity
        zDir = zVelocity
    end
		
	local attackSpeed = 5.0
	local playerVelocity = gameObject:GetBody().velocity
	body.velocity = playerVelocity + attackSpeed * vec3.new(xDir, 0.0, zDir)
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

function PlayerAttack ()
	for i=1,3 do
		CreateFireball(i, i)
	end
	--PlaySFX("Resources/Audio/SFX/PlayerAttack.wav")
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	local particle = gameObject:GetParticleEmitter()
	if attack.shouldAttack then
		PlayerAttack()
		attack.shouldAttack = false
		attack.currentAttackTime = 0.0
	end
	if attack.currentAttackTime >= 0.5 then
		local particle = gameObject:GetParticleEmitter()
		if particle then
			particle.active = false
		end
	end
end

CheckValidAttack()