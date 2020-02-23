function CreateLargeFireball()
	local go = CreatePrefabAtPosition("LargeFireball", gameObject:GetTransform().position)
	local body = go:GetBody()
		
	local xVelocity = GetAxis(CONTROLLER["RX"])
	local zVelocity = GetAxis(CONTROLLER["RY"])
	local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
	xVelocity = xVelocity / totalVelocity
	zVelocity = zVelocity / totalVelocity
		
	local attackSpeed = 25.0
	body.velocity = attackSpeed * vec3.new(xVelocity, 0.0, zVelocity)

	transform = go:GetTransform()
	transform.position = body.position

	-- Change Color
	material = go:GetMaterial()
	material.diffuse = vec3.new(4.0, 0.0, 0.0)
end

function PlayerAttack ()
	-- Create a large fireball
	CreateLargeFireball()
	PlaySFX("Resources/Audio/SFX/PlayerAttack.wav")
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if (attack.shouldAttack == true) then
		PlayerAttack()
		attack.shouldAttack = false
	end
end

CheckValidAttack()