function SpawnEarthSpell ()
	local go = CreatePrefabAtPosition("EarthSpell", gameObject:GetTransform().position)
	go:GetBody().position.y = 1.5
	go:GetTransform().position.y = go:GetBody().position.y
end

SpawnEarthSpell()