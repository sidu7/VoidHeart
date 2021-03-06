function ScaleZ()
	-----------------------------------------
    -- playtesting vars
    -----------------------------------------
	
	-- Cast a ray from the player to get the nearest object
	if (player == nil and player.isActive ~= true) then
        return
    end
    local playerTransform = player:GetTransform()
    local playerPos = playerTransform.position
    local direction = playerTransform:forward()

    local depth = DCastRay(playerPos, direction)
	
	local body = gameObject:GetBody()
	local front = VecNormalize(player:GetTransform():forward())
	body.position = player:GetTransform().position + vec3.new(front.x*(depth/2.0), 0.0, front.z*(depth/2.0))
	

	-- Update shape radius for physics
	local transform = gameObject:GetTransform()
	transform.position = body.position
	transform.scale.z = depth
	local particle = gameObject:GetParticleEmitter()
	if particle ~= nil then
		particle.extraData.y = depth
	end
	ChangeRadius(gameObject)
end

ScaleZ()