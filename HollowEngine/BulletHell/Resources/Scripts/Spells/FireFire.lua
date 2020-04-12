function CreateLargeFireball()
	local go = CreatePrefabAtPosition("LargeFireball", gameObject:GetTransform().position)
	local body = go:GetBody()
		
	local transform = gameObject:GetTransform()
    local rot = transform.rotation
    local angle = rot.y
    angle = angle * math.pi / 180
    local xDir = math.sin(angle)
    local zDir = math.cos(angle)

	local xVelocity = GetAxis(CONTROLLER["RX"])
	local zVelocity = GetAxis(CONTROLLER["RY"])
    if ((xVelocity < - 16000) or (xVelocity > 16000) or (zVelocity < -16000) or (zVelocity > 16000)) then 
        local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
        xVelocity = xVelocity / totalVelocity
        zVelocity = zVelocity / totalVelocity
        xDir = xVelocity
        zDir = zVelocity
    end
		
	local attackSpeed = 5.0
	body.velocity = attackSpeed * vec3.new(xDir, 1.2, zDir)
	body.position = transform.position + vec3.new(0.0, 0.75, 0.0)

	transform = go:GetTransform()
	transform.position = body.position

	-- Change Color
	material = go:GetMaterial()
	material.diffuse = vec3.new(4.0, 0.0, 0.0)
end

function PlayerAttack ()
	-- Create a large fireball
	CreateLargeFireball()
	PlaySFX("Resources/Audio/SFX/FireFireSpellAttack.wav")
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if (attack.shouldAttack == true) then
		PlayerAttack()
		attack.shouldAttack = false
	end
end

CheckValidAttack()