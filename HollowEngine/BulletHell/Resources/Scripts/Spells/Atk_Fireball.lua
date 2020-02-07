function PlayerAttack ()
	attackPosition = gameObject:GetTransform().position
	---[[
	attack = gameObject:GetAttack()
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
	end--]]
end

function CheckValidAttack()
	--local hThreshold = 800
	--local vThreshold = 80
	
	-- May need to rethink this
	--local rightHorizontalAxis = GetAxis(CONTROLLER["LX"])
	--local rightVerticalAxis = GetAxis(CONTROLLER["LY"])
	
	-- Debug input for keyboard
	local debugFire = (IsControllerTriggerTriggered(CONTROLLER["LT"]) or IsControllerTriggerTriggered(CONTROLLER["RT"]))
	if (debugFire) then
		rightHorizontalAxis = 10000
		rightVerticalAxis = 0
	end
	
	
	--if math.abs(rightHorizontalAxis) > hThreshold
	--or math.abs(rightVerticalAxis) > hThreshold 
	--or debugFire then
	if debugFire then
		PlayerAttack()
	end
end

CheckValidAttack()