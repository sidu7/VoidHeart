function SlowRotate()
	local transform = gameObject:GetTransform()
	local rot = transform.rotation
	transform:Rotate(vec3.new(rot.x, rot.y+1.0, rot.z))
end

function BobUpAndDown()
	local maxUp = 1.4
	local minDown = 1.0
	local rate = 0.005
	local body = gameObject:GetBody()
	local spell = gameObject:GetSpell()
	-- Going up but should now go down
	if body.position.y > maxUp then
		spell.goingUp = false
	end
	-- Going down
	if body.position.y < minDown then
		spell.goingUp = true
	end
	if not spell.goingUp then
		rate = -rate
	end
	body.position.y = body.position.y + rate
end

function Update()
	SlowRotate()
	BobUpAndDown()
end

Update()