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
	if math.abs(horizontalAxis) > hThreshold
	or math.abs(verticalAxis) > hThreshold then
		--PlayerAttack()
		FirePattern2()
	end
end

function FirePattern2 ()
	gameObjectPath = "Resources/Json data/PlayerBullet.json"
	local numObj = 3
	for i=1,numObj do
		go = CreateGameObject(gameObjectPath)
		body = go:GetBody()
		body.position = attackPosition + vec3.new(i*0.1, 0.0, 0.3*i+0.8)
		body.velocity = vec3.new(0.0, 0.0, 20.0)
	end
end

CheckValidAttack()
