function Update()
    
	local body = gameObject:GetBody()
    body.velocity = vec3.new(0.0, 0.0, 0.0)

    -- collision handling
    if (dirLength < 1.0) then
        local impulseStrength = vec3.new(1000, 0, 1000)
        local impulse = vec3.new(xDirNorm, 0.0, zDirNorm)
        impulse  = impulse * impulseStrength
        ApplyLinearImpulse(player, impulse)

        local impulseReverse = vec3.new(-xDirNorm, 0.0, -zDirNorm)
        impulseReverse = impulseReverse * impulseStrength
        ApplyLinearImpulse(gameObject, impulseReverse)
    end
end

Update()