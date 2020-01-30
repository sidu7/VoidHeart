--Default attack script before gaining any spells
function FireAttack()
	local impulse = vec3.new()
	if (IsControllerTriggerTriggered(CONTROLLER["LT"]) or IsControllerTriggerTriggered(CONTROLLER["RT"])) then
		dashSpeed = 1500.0
		body = gameObject:GetBody()
		-- Damp Overall Velocity and Rotation
		local direction = body.velocity
		local magnitude = math.sqrt(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z)
		direction.x = direction.x / magnitude
		direction.y = direction.y / magnitude
		direction.z = direction.z / magnitude
		impulse = impulse + direction * dashSpeed
		PlaySFX("Resources/Audio/SFX/PlayerDash.wav")
		ApplyLinearImpulse(gameObject, impulse)
	end
end

FireAttack()