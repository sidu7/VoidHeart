function PlayerAttack ()
	if attack.currentAttackTime > attack.baseAttackTime then
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
		
		attack.currentAttackTime = 0.0
		PlaySFX("Resources/Audio/SFX/PlayerAttack.wav")
	end
end

function CheckValidAttack()
	local hThreshold = 8000
	local vThreshold = 8000
	if math.abs(GetAxis(CONTROLLER["RX"])) > hThreshold
	or math.abs(GetAxis(CONTROLLER["RY"])) > hThreshold then
		PlayerAttack()
	end
end

CheckValidAttack()
