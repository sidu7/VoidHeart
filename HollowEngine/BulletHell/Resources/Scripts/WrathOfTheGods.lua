function Push()
	-----------------------------------------
    -- playtesting vars
	local maxScale = 10.0
	local scaleRate = 16.0 -- percent increase per frame
    -----------------------------------------
	local destroy = gameObject:GetDestroyTimer()
	local transform = gameObject:GetTransform()
	local particles = gameObject:GetParticleEmitter()
	local change = 1.0 + scaleRate / 100.0
	local body = gameObject:GetBody()

	-- scale it in looks only
	if (transform.scale.x < maxScale) then
		-- Update Scale
		transform.scale = transform.scale * change
		-- Update Particles
		particles.areaOfEffect = particles.areaOfEffect * (1.0 + scaleRate / 500.0)
	end

	if transform.scale.x > maxScale then
		isScaled = true
	else
		isScaled = false
	end
	
	if isScaled then
		local room = GetDungeonFloor(currentFloor):GetRoomFromIndex(currentRoom)
		local enemies = room.Enemies
		for k=1,#enemies do
			local enemyDistance = VecLength(body.position - enemies[k]:GetTransform().position)
			if enemyDistance < maxScale/2 then
				-- apply Impulse
				local impulse = body.position - enemies[k]:GetTransform().position
				impulse.x = -impulse.x 
				impulse.z = -impulse.z
				impulse.y = 0
				ApplyLinearImpulse(enemies[k], impulse)

				-- apply Damage
				HandleBulletDamage(enemies[k], gameObject, false)
			end
		end

		isScaled = false
	end
end

Push()