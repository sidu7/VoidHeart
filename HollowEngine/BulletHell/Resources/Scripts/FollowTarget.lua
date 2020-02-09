function FollowTarget()
	local attack = gameObject:GetAttack()
	local target = attack.target
	local position = gameObject:GetTransform().position
	if target ~= nil then
	if target:GetTransform() then
		local targetTransform = target:GetTransform()
		local targetPos = targetTransform.position

		-- calculate direction
		local xDir = targetPos.x - position.x
		local zDir = targetPos.z - position.z
		local dirLength = math.sqrt(xDir*xDir + zDir*zDir)
		local xDirNorm = xDir / dirLength
		local zDirNorm = zDir / dirLength
		
		local body = gameObject:GetBody()
		local speed = 6.0
		body.velocity = speed * vec3.new(xDirNorm, 0.0, zDirNorm)
	end
	end
end

FollowTarget()