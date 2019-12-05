function FollowPlayerAttack()
	body = followObject:GetBody()
	
	---[[
	local xVelocity = followPosition.x - body.position.x
	local zVelocity = followPosition.z - body.position.z
	local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
	xVelocity = xVelocity / totalVelocity
	zVelocity = zVelocity / totalVelocity
	
	local attackSpeed = 10.0
	body.velocity = attackSpeed * vec3.new(xVelocity, 0.0, zVelocity)
	--]]
	
end

function Normal()
-- Change Color
	material = followObject:GetMaterial()
	material.diffuse = vec3.new(1.0, 1.0, 1.0)
end

function Update()
if (attack.currentAttackTime <  attack.baseAttackTime) then
		FollowPlayerAttack()
		else
			Normal()
	end
end

Update()