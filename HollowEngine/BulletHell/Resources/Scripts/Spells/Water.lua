function CreateHomingWave()
	-----------------------------------------
    -- playtesting vars
	local attackSpeed = 10.0
	-----------------------------------------
	local attackPosition = gameObject:GetTransform().position

	local gameObjectPath = "Resources/Prefabs/PlayerWater.json"
	local go = CreateGameObject(gameObjectPath)
	local body = go:GetBody()
	body.position = attackPosition
		
	local xVelocity = GetAxis(CONTROLLER["RX"])
	local zVelocity = GetAxis(CONTROLLER["RY"])
	local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
	xVelocity = xVelocity / totalVelocity
	zVelocity = zVelocity / totalVelocity
	
	body.velocity = attackSpeed * vec3.new(xVelocity, 0.0, zVelocity)

	local transform = go:GetTransform()
	transform.position = body.position
	
	-- Get the current room/enemies
	local floor = GetDungeonFloor(currentFloor)
	local currentRoomObj = floor:GetRoomFromIndex(currentRoom)
	local enemies = currentRoomObj.Enemies
	
	-- Find enemy closest to the player for now
	local closestEnemyIndex = 0
	local closestEnemyDistance = 100000000.0
	for k=1,#enemies do
		local enemyDistance = VecLength(body.position - enemies[k]:GetTransform().position)
		if enemyDistance < closestEnemyDistance then
			closestEnemyIndex = k
			closestEnemyDistance = enemyDistance
		end
	end
	if closestEnemyIndex > 0 then
		go:GetAttack().target = enemies[closestEnemyIndex]
	end
	--PlaySFX("Resources/Audio/SFX/PlayerAttack.wav")
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		CreateHomingWave()
		attack.shouldAttack = false
	end
end

CheckValidAttack()