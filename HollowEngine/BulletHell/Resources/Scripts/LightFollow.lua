function SetLightPosition()
	-----------------------------------------
    -- playtesting vars
	local positionOffset = vec3.new(3.0, 5.0, 2.0)
	local lookAtOffset = vec3.new(0.0, 0.0, 0.0)
	-----------------------------------------	
	local transform = gameObject:GetTransform()
	
	-- Light position is 
	transform.position = camera:GetTransform().position + positionOffset
	local light = gameObject:GetLight()
	light.lookAt = player:GetTransform().position + lookAtOffset
end

function UpdateLight()
	SetLightPosition()
end

UpdateLight()