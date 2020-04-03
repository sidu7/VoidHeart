function RotateWithPlayer()
	-----------------------------------------
    -- playtesting vars
	local rotateOffset = vec3.new(0.0, 270.0, 0.0)
    -----------------------------------------
	 local body = gameObject:GetBody()
	body:RotateBody(player:GetTransform().rotation + rotateOffset)
end

RotateWithPlayer()