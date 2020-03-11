function QuickGrow()
	-----------------------------------------
    -- playtesting vars
	local maxScale = 1.0
	local scaleRate = 22.0 -- percent increase per frame
    -----------------------------------------
	local destroy = gameObject:GetDestroyTimer()
	local transform = gameObject:GetTransform()
	local change = 1.0 + scaleRate / 100.0
	if (transform.scale.x < maxScale) then
		-- Update Scale
		transform.scale = transform.scale * change
	else
		--local material = gameObject:GetMaterial()
		--local deltaTime = GetFrameTime()
		--local frameRemain = (destroy.maxTime - destroy.currentTime)/deltaTime
		--material.alphaValue = material.alphaValue - material.alphaValue / frameRemain
	end
	local body = gameObject:GetBody()
	-- Update shape radius for physics
	if body then
		ChangeRadius(gameObject)
	end
end

QuickGrow()