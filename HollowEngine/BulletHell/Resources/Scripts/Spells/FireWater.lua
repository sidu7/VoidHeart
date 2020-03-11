function CreateSteamTrap()
	local go = CreatePrefabAtPosition("SteamTrap", gameObject:GetTransform().position)
	local body = go:GetBody()
	
	transform = go:GetTransform()
	transform.position = body.position

	local particle = go:GetParticleEmitter()
	if particle then
		particle.extraData.x = transform.scale.x;
	end
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if (attack.shouldAttack == true) then
		CreateSteamTrap()
		attack.shouldAttack = false
	end
end

CheckValidAttack()