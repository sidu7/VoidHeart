function PlayerAttack ()
	if attack.currentAttackTime > attack.baseAttackTime then
		local gameObjectPath = "Resources/Json data/PlayerBullet.json"
		go = CreateGameObject(gameObjectPath)
		body = go:GetBody()
		body.position = attackPosition
		
		local xVelocity = horizontalAxis
		local zVelocity = verticalAxis
		local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
		xVelocity = xVelocity / totalVelocity
		zVelocity = zVelocity / totalVelocity
		
		local attackSpeed = 20.0
		body.velocity = attackSpeed * vec3.new(xVelocity, 0.0, zVelocity)

		transform = go:GetTransform()
		transform.position = body.position
		
		attack.currentAttackTime = 0.0
	end
end

function CheckValidAttack()
	local hThreshold = 8000
	local vThreshold = 8000
	if math.abs(horizontalAxis) > hThreshold
	or math.abs(verticalAxis) > hThreshold then
		PlayerAttack()
	end
end

CheckValidAttack()
