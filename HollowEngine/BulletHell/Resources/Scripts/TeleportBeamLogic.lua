function Interpolate(current, a, b)
    return (current - a) / (b - a)
end

function Update()
	-----------------------------------------
    -- playtesting vars
    -----------------------------------------
    local beamHeight = 9
    -----------------------------------------

    local destroyTimer = gameObject:GetDestroyTimer()
    local particle = gameObject:GetParticleEmitter()
    if particle == nil then
        return
    end

	local playerPos = player:GetTransform().position
    local playerScaleY = player:GetTransform().scale.y

    if (destroyTimer.currentTime > destroyTimer.maxTime / 2) then
	    local position = vec3.new(playerPos.x, playerPos.y - playerScaleY / 2 + beamHeight, playerPos.z)
		particle.extraData.y = beamHeight * (1 - Interpolate(destroyTimer.currentTime, destroyTimer.maxTime / 2, destroyTimer.maxTime))
	    particle.center = position
    else--if (destroyTimer.currentTime > destroyTimer.maxTime / 3) then
	    local position = vec3.new(playerPos.x, playerPos.y - playerScaleY / 2 + beamHeight, playerPos.z)
        particle.extraData.y = beamHeight
	    particle.center = position
    -- uncomment and modify above to have 3 phases of teleport
    --[[else 
	    local position = vec3.new(playerPos.x, playerPos.y - playerScaleY / 2 + beamHeight, playerPos.z)
		particle.extraData.y = beamHeight * Interpolate(destroyTimer.currentTime, 0, destroyTimer.maxTime / 3)
	    particle.center = position
    --]]
    end
	particle.extraData.z = particle.extraData.z + GetFrameTime()
    if (destroyTimer.currentTime < destroyTimer.maxTime * 9 / 10) then
        player:GetBody().velocity = vec3.new(0.0, 0.0, 0.0)
    end
end

Update()