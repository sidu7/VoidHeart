function SpawnEarthSpell ()
	local go = CreatePrefabAtPosition("EarthSpell", gameObject:GetTransform().position)
	go:GetTransform().position.y = 0.5
end

SpawnEarthSpell()