function GrowWall()
	-----------------------------------------
    -- playtesting vars
	local maxScale = vec3.new(3.0, 1.5, 1.0)
	local scaleRate = 10.0 -- percent increase per frame
    -----------------------------------------
	
	local transform = gameObject:GetTransform()
	local body = gameObject:GetBody()
	--local particles = gameObject:GetParticleEmitter()
	local change = 1.0 + scaleRate / 100.0
	-- No rotation hacky fix
	body:RotateBody(transform.rotation)
	
	-- Update scales
	if (transform.scale.x < maxScale.x) then
		--transform.scale.x = transform.scale.x * change
		transform.scale.x = maxScale.x
	end

	if (transform.scale.y < maxScale.y) then
		transform.scale.y = transform.scale.y * change
	end
	
	if (transform.scale.z < maxScale.z) then
		--transform.scale.z = transform.scale.z * change
		transform.scale.z = maxScale.z
	end
	
	transform.position.y = transform.scale.y * change
	
	--[[
	else
		-- Update Particles
		--particles.areaOfEffect = particles.areaOfEffect * (1.0 + scaleRate / 500.0)
		local material = gameObject:GetMaterial()
		local deltaTime = GetFrameTime()
		local frameRemain = (destroy.maxTime - destroy.currentTime)/deltaTime
		--material.alphaValue = material.alphaValue - material.alphaValue / frameRemain
	end
	--]]
	-- Update shape radius for physics
	if body then
		-- Function actually calls update scale so should be ok for BOX
		ChangeRadius(gameObject)
	end
end

GrowWall()