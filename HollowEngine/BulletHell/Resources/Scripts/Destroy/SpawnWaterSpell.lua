function SpawnWaterSpell ()
	local go = CreatePrefabAtPosition("WaterSpell", gameObject:GetTransform().position)
	go:GetBody().position.y = 1.5
	go:GetTransform().position.y = go:GetBody().position.y
end

SpawnWaterSpell()