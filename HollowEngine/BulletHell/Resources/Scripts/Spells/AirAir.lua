function FireAttack()
	local go = CreatePrefabAtPosition("PlayerAOE", gameObject:GetTransform().position)
	local particle = gameObject:GetParticleEmitter()
	if particle then
		particle.active = false
	end
	PlaySFX("Resources/Audio/SFX/AirAirSpellAttack.wav")
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		FireAttack()
		attack.shouldAttack = false
	end
end

CheckValidAttack()