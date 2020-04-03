function SpawnFireSpell ()
	local go = CreatePrefabAtPosition("FireSpell", gameObject:GetTransform().position)
	go:GetTransform().position.y = 0.5
end

SpawnFireSpell()