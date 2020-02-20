function AirDash()
    -----------------------------------------
    -- playtesting vars
	local dashSpeed = 1000.0
    -----------------------------------------

	local body = gameObject:GetBody()
	-- Damp Overall Velocity and Rotation
	local direction = body.velocity
	local magnitude = math.sqrt(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z)
	if magnitude > 0.001 then
		direction.x = direction.x / magnitude
		direction.y = direction.y / magnitude
		direction.z = direction.z / magnitude
		local impulse = vec3.new(0.0, 0.0, 0.0)
		impulse = impulse + direction * dashSpeed
		impulse.y = 0.0
		PlaySFX("Resources/Audio/SFX/PlayerDash.wav")
		ApplyLinearImpulse(gameObject, impulse)
	end
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		AirDash()
		attack.currentAttackTime = 0.0
		attack.shouldAttack = false
	end
end

CheckValidAttack()