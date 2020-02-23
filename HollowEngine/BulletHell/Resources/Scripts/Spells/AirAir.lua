function FireAttack()
	local go = CreatePrefabAtPosition("PlayerAOE", gameObject:GetTransform().position)
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		FireAttack()
		attack.shouldAttack = false
	end
end

CheckValidAttack()