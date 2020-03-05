function CreateBeam()
	local go = CreatePrefabAtPosition("PlayerBeam", gameObject:GetTransform().position)
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		CreateBeam()
		--CastRay()
		attack.shouldAttack = false
	end
end

CheckValidAttack()