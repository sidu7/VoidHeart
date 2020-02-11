function grow()
    -----------------------------------------
    -- playtesting vars
	local scaleRate = 1.02
    -----------------------------------------
	local transform = gameObject:GetTransform()
	transform.scale = transform.scale * scaleRate
	local body = gameObject:GetBody()
	-- Update shape radius for physics
	if body then
		ChangeRadius(gameObject)
	end
end

grow()