function PlayerAttack ()
	local attackPosition = gameObject:GetTransform().position
	---[[
	local attack = gameObject:GetAttack()
	if attack.currentAttackTime > attack.baseAttackTime then
		local gameObjectPath = "Resources/Json data/PlayerBullet.json"
		local go = CreateGameObject(gameObjectPath)
		local body = go:GetBody()
		body.position = attackPosition
		
		local xVelocity = GetAxis(CONTROLLER["RX"])
		local zVelocity = GetAxis(CONTROLLER["RY"])
		local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
		xVelocity = xVelocity / totalVelocity
		zVelocity = zVelocity / totalVelocity
		
		local attackSpeed = 70.0
		body.velocity = attackSpeed * vec3.new(xVelocity, 0.0, zVelocity)

		local transform = go:GetTransform()
		transform.position = body.position
		
		attack.currentAttackTime = 0.0
		PlaySFX("Resources/Audio/SFX/PlayerAttack.wav")
	end--]]
end

function CheckValidAttack()
	local hThreshold = 8000
	local vThreshold = 8000
	
	-- May need to rethink this
	local rightHorizontalAxis = GetAxis(CONTROLLER["RX"])
	local rightVerticalAxis = GetAxis(CONTROLLER["RY"])
	
	-- Debug input for keyboard
	local debugFire = IsKeyPressed("H")
	if (debugFire) then
		rightHorizontalAxis = 10000
		rightVerticalAxis = 0
	end
	
	
	if math.abs(rightHorizontalAxis) > hThreshold
	or math.abs(rightVerticalAxis) > hThreshold 
	or debugFire then
		PlayerAttack()
	end
end

CheckValidAttack()