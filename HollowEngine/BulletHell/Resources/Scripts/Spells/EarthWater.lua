function CreateBarrier()
	local gameObjectPath = "Resources/Prefabs/PlayerWall.json"
	local wallObject = CreateGameObject(gameObjectPath)
	local transform = wallObject:GetTransform()
	
	local body = wallObject:GetBody()
	local spawnPosition = gameObject:GetTransform().position
	local forward = gameObject:GetTransform():forward()
	body.position = spawnPosition + forward
	transform.position = body.position
	body:RotateBody(gameObject:GetTransform().rotation)
	PlaySFX("Resources/Audio/SFX/EarthWaterSpellAttack.wav")
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		CreateBarrier()
		attack.shouldAttack = false
	end
end

CheckValidAttack()