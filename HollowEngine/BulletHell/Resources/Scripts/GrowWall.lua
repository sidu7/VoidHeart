function Grow()
	-----------------------------------------
    -- playtesting vars
	local maxScale = vec3.new(3.0, 3.0, 1.0)
	local scaleRate = 16.0 -- percent increase per frame
    -----------------------------------------
	
	
	local destroy = gameObject:GetDestroyTimer()
	local transform = gameObject:GetTransform()
	--local particles = gameObject:GetParticleEmitter()
	local change = 1.0 + scaleRate / 100.0
	-- No rotation hacky fix
	
	transform:Rotate(transform.rotation)
	-- Update scales
	if (transform.scale.x < maxScale.x) then
		transform.scale.x = transform.scale.x * change
	end

	if (transform.scale.y < maxScale.y) then
		transform.scale.y = transform.scale.y * change
	end
	
	if (transform.scale.z < maxScale.z) then
		transform.scale.z = transform.scale.z * change
	end
	
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
	local body = gameObject:GetBody()
	-- Update shape radius for physics
	if body then
		-- Function actually calls update scale so should be ok for BOX
		ChangeRadius(gameObject)
	end
end

Grow()