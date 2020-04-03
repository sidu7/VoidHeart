function SpawnEarthSpell ()
	local go = CreatePrefabAtPosition("EarthSpell", gameObject:GetTransform().position)
	go:GetBody().position.y = player:GetBody().position.y
	go:GetTransform().position.y = go:GetBody().position.y
end

SpawnEarthSpell()