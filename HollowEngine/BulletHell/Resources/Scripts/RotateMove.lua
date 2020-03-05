function RotateMove()
	-----------------------------------------
    -- playtesting vars
	local radius = 1.5
    -----------------------------------------
	
	-- Calculate right vectors
	local playerTransform = player:GetTransform()
	local front = playerTransform:forward()
	
	local body = gameObject:GetBody()
	local playerPosition = playerTransform.position
	-- Calculate rotation due to time
	local destroyTimer = gameObject:GetDestroyTimer()
	local t = destroyTimer.currentTime
	local maxT = destroyTimer.maxTime
	local alpha = t / maxT * math.pi/2.0
	
	-- Calculate rotation from front vector
	local tangent = front.x / front.z
    local beta = math.atan(tangent)
    if front.z >= 0 then
		beta = beta + math.pi
    end
	
	-- Rotate left or right based on GO tag
	if gameObject.tag == "RHWave" then
		body.position.x = playerPosition.x + radius*math.cos(-alpha - beta)
		body.position.z = playerPosition.z + radius*math.sin(-alpha - beta)		
	else
		body.position.x = playerPosition.x - radius*math.cos(alpha - beta)
		body.position.z = playerPosition.z - radius*math.sin(alpha - beta)
	end
end

RotateMove()