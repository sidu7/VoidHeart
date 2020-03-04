function DropMeteors()
	for i=0, 10, 1 do
		local go = CreatePrefabAtPosition("Meteor", gameObject:GetTransform().position)
		local body = go:GetBody()
		
		local transform = gameObject:GetTransform()
		local rot = transform.rotation
		local angle = rot.y
		angle = angle * math.pi / 180
		local xDir = math.sin(angle)
		local zDir = math.cos(angle)

		local xOffset = math.random(-2, 2)
		local zOffset = math.random(-2, 2)

		body.position = transform.position + vec3.new(6.0 * xDir + xOffset, 15.0 + 5.0 * i, 6.0 * zDir + zOffset)
	
		local attackSpeed = 20.0
		body.velocity = attackSpeed * vec3.new(0.0, -1.0, 0.0)

		transform = go:GetTransform()
		transform.position = body.position
	end
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if (attack.shouldAttack == true) then
		DropMeteors()
		attack.shouldAttack = false
	end
end

CheckValidAttack()