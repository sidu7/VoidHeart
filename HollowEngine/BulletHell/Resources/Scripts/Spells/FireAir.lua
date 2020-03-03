function CreateFireball(xVel, zVel)
	local go = CreatePrefabAtPosition("PlayerFlames", gameObject:GetTransform().position)
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
	local playerVelocity = gameObject:GetBody().velocity
	body.velocity = playerVelocity + attackSpeed * vec3.new(xDir, 0.0, zDir)
	-- Add player velocity

	transform = go:GetTransform()
	transform.position = body.position

	-- Change Color
	material = go:GetMaterial()
	material.diffuse = vec3.new(4.0, 0.0, 0.0)
end

function PlayerAttack ()
	for i=1,3 do
		CreateFireball(i, i)
	end
	--PlaySFX("Resources/Audio/SFX/PlayerAttack.wav")
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		PlayerAttack()
		attack.shouldAttack = false
	end
end

CheckValidAttack()