function Update()
    
	local body = gameObject:GetBody()
    body.velocity = vec3.new(0.0, 0.0, 0.0)
end

Update()