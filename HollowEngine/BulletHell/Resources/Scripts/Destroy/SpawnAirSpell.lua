function SpawnAirSpell ()
	local go = CreatePrefabAtPosition("AirSpell", gameObject:GetTransform().position)
	go:GetBody().position.y = 1.5
	go:GetTransform().position.y = go:GetBody().position.y
end

SpawnAirSpell()