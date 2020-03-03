function CreateRock()
	local gameObjectPath = "Resources/Prefabs/PlayerRock.json"
	rockObject = CreateGameObject(gameObjectPath)
	local transform = rockObject:GetTransform()
	
	local body = rockObject:GetBody()
	local spawnPosition = gameObject:GetTransform().position
	body.position = spawnPosition

end

function ChargeRock()
	local maxChargeTime = 3.0
	local attack = gameObject:GetAttack()
	attack.chargeTime = attack.chargeTime + 0.05 -- TODO: Change this to delta time
	if attack.chargeTime > maxChargeTime then
		attack.chargeTime = maxChargeTime
	end
	local transform = rockObject:GetTransform()
	local body = rockObject:GetBody()
	if body then
		body.position = gameObject:GetTransform().position
		local forward = gameObject:GetTransform():forward()
		body.position = body.position + forward
	end
	if transform then
		transform.scale = vec3.new(attack.chargeTime, attack.chargeTime, attack.chargeTime)
	end
	-- Update shape radius for physics
	if body then
		ChangeRadius(rockObject)
	end
end

function ReleaseRock()
	local body = rockObject:GetBody()
	if body then
    
        local transform = gameObject:GetTransform()
        local rot = transform.rotation
        local angle = rot.y
        angle = angle * math.pi / 180
        local xDir = math.sin(angle)
        local zDir = math.cos(angle)

		local xVelocity = GetAxis(CONTROLLER["RX"])
	    local zVelocity = GetAxis(CONTROLLER["RY"])
        if ((xVelocity < - 16000) or (xVelocity > 16000) or (zVelocity < -16000) or (zVelocity > 16000)) then 
            local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
            xVelocity = xVelocity / totalVelocity
            zVelocity = zVelocity / totalVelocity
            xDir = xVelocity
            zDir = zVelocity
        end
		
		local attackSpeed = 70.0
		body.velocity = attackSpeed * vec3.new(xDir, 0.0, zDir)
	end
	rockObject = nil
	
	local attack = gameObject:GetAttack()
	attack.chargeTime = 0.0
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	
	if rockObject ~= nil then
		-- Charge Rock, done every frame
		ChargeRock()
		
		-- Release Rock, done when trigger is released
		if IsControllerTriggerReleased(CONTROLLER["LT"]) or IsControllerTriggerReleased(CONTROLLER["RT"]) then
			ReleaseRock()
		end		
	end
	
	if attack.shouldAttack then
		-- Create Rock, only done once
		if not rockObject then
			CreateRock()
		end
	end
end

CheckValidAttack()