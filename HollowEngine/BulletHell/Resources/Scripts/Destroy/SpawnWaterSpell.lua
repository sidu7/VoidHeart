function SpawnWaterSpell ()
	local go = CreatePrefabAtPosition("WaterSpell", gameObject:GetTransform().position)
	go:GetBody().position.y = player:GetBody().position.y
	go:GetTransform().position.y = go:GetBody().position.y
end

SpawnWaterSpell()