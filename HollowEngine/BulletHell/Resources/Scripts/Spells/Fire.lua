function PlayerAttack ()
	attackPosition = gameObject:GetTransform().position

	local gameObjectPath = "Resources/Json data/PlayerBullet.json"
	go = CreateGameObject(gameObjectPath)
	body = go:GetBody()
	body.position = attackPosition
		
	local xVelocity = GetAxis(CONTROLLER["RX"])
	local zVelocity = GetAxis(CONTROLLER["RY"])
	local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
	xVelocity = xVelocity / totalVelocity
	zVelocity = zVelocity / totalVelocity
		
	local attackSpeed = 70.0
	body.velocity = attackSpeed * vec3.new(xVelocity, 0.0, zVelocity)

	transform = go:GetTransform()
	transform.position = body.position
		
	PlaySFX("Resources/Audio/SFX/PlayerAttack.wav")
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		PlayerAttack()
		attack.currentAttackTime = 0.0
		attack.shouldAttack = false
	end
end

CheckValidAttack()