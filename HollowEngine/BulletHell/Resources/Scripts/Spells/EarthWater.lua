function CreateBarrier()
	local gameObjectPath = "Resources/Prefabs/PlayerWall.json"
	local wallObject = CreateGameObject(gameObjectPath)
	local transform = wallObject:GetTransform()
	
	local body = wallObject:GetBody()
	local spawnPosition = gameObject:GetTransform().position
	local forward = gameObject:GetTransform():forward()
	body.position = spawnPosition + forward
	transform.position = body.position
	transform:Rotate(gameObject:GetTransform().rotation)

end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		CreateBarrier()
		attack.shouldAttack = false
	end
end

CheckValidAttack()