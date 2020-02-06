function FireAttack()
	attackPosition = gameObject:GetTransform().position

	local gameObjectPath = "Resources/Json data/PlayerAOE.json"
	local go = CreateGameObject(gameObjectPath)
	body = go:GetBody()
	body.position = attackPosition
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		FireAttack()
		attack.shouldAttack = false
	end
end

CheckValidAttack()