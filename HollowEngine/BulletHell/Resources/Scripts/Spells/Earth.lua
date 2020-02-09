function MeleeAttack ()
	-- Create melee collision object
	
	local melee = CreatePrefabAtPosition("MeleeCollider", gameObject:GetTransform().position)

end

function ShortDash()
	local impulse = vec3.new()
	local dashSpeed = 1000.0
	local body = gameObject:GetBody()
	-- Damp Overall Velocity and Rotation
	local direction = body.velocity
	local magnitude = math.sqrt(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z)
	direction.x = direction.x / magnitude
	direction.y = direction.y / magnitude
	direction.z = direction.z / magnitude
	impulse = impulse + direction * dashSpeed
	impulse.y = 0.0
	PlaySFX("Resources/Audio/SFX/EarthMelee.wav")
	ApplyLinearImpulse(gameObject, impulse)
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		ShortDash()
		MeleeAttack()
		attack.shouldAttack = false
	end
end

CheckValidAttack()