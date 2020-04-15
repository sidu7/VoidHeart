function SpawnHoming()
	-----------------------------------------
    -- playtesting vars
	-----------------------------------------
	local bulletPrefabPath = "Resources/Json data/BossWaterBullet.json"
	local bullet = CreateGameObject(bulletPrefabPath)
	local bulletBody = bullet:GetBody()
	bulletBody.position = gameObject:GetTransform().position
	PlaySFX("Resources/Audio/SFX/BossWaterHoming.wav")
end

SpawnHoming()