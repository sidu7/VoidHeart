function PortalScale()
	-----------------------------------------
    -- playtesting vars
	local maxScale = vec3.new(1.7, 2.9, 1.7)
	local scaleRate = 5.0 -- percent increase per frame
    -----------------------------------------
	local transform = gameObject:GetTransform()
	local change = 1.0 + scaleRate / 100.0
	-- Update Scale
	if (transform.scale.x < maxScale.x) then
		transform.scale.x = transform.scale.x * change
	end
	if (transform.scale.y < maxScale.y) then
		transform.scale.y = transform.scale.y * change
	end
	if (transform.scale.z < maxScale.z) then
		transform.scale.z = transform.scale.z * change
	end
	
	local body = gameObject:GetBody()
	-- Update shape radius for physics
	if body then
		--ChangeRadius(gameObject)
	end
end

function PortalGrow()
	PortalScale()
end

PortalGrow()