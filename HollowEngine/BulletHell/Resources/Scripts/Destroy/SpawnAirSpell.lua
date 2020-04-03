function SpawnAirSpell ()
	local go = CreatePrefabAtPosition("AirSpell", gameObject:GetTransform().position)
	go:GetTransform().position.y = 0.5
end

SpawnAirSpell()