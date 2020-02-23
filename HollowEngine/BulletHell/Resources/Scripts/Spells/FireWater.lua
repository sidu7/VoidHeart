function CreateSteamTrap()
	local go = CreatePrefabAtPosition("SteamTrap", gameObject:GetTransform().position)
	local body = go:GetBody()

	transform = go:GetTransform()
	transform.position = body.position
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if (attack.shouldAttack == true) then
		CreateSteamTrap()
		attack.shouldAttack = false
	end
end

CheckValidAttack()