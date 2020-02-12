function grow()
    -----------------------------------------
    -- playtesting vars
	local maxScale = 10.0
	local scaleRate = 16.0 -- percent increase per frame
    -----------------------------------------
	local destroy = gameObject:GetDestroyTimer()
	local transform = gameObject:GetTransform()
	local particles = gameObject:GetParticleEmitter()
	local change = 1.0 + scaleRate / 100.0
	if (transform.scale.x < maxScale) then
		-- Update Scale
		transform.scale = transform.scale * change
		-- Update Particles
		particles.areaOfEffect = particles.areaOfEffect * (1.0 + scaleRate / 500.0)
	else
		local material = gameObject:GetMaterial()
		local deltaTime = GetFrameTime()
		local frameRemain = (destroy.maxTime - destroy.currentTime)/deltaTime
		--material.alphaValue = material.alphaValue - material.alphaValue / frameRemain
	end
	local body = gameObject:GetBody()
	-- Update shape radius for physics
	if body then
		ChangeRadius(gameObject)
	end

end

grow()