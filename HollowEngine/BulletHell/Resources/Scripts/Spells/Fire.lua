function PlayerAttack ()
	local go = CreatePrefabAtPosition("PlayerFire", gameObject:GetTransform().position)
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

	local attackSpeed = 15.0
	body.velocity = attackSpeed * vec3.new(xDir, 0.0, zDir)
	local transform = go:GetTransform()
	transform.position = body.position
		
	local particle = go:GetParticleEmitter()
	particle.direction = vec3.new(-xDir, 0.0, -zDir);

	PlaySFX("Resources/Audio/SFX/PlayerAttack.wav")
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		PlayerAttack()
		attack.currentAttackTime = 0.0
		attack.shouldAttack = false
	end
end

CheckValidAttack()