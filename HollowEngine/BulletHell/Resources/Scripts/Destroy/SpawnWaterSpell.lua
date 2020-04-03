function SpawnWaterSpell ()
	local go = CreatePrefabAtPosition("WaterSpell", gameObject:GetTransform().position)
	go:GetTransform().position.y = 0.5
end

SpawnWaterSpell()