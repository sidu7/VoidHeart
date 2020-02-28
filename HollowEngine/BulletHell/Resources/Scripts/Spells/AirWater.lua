function CreateIceWave()
	local lhWave = CreatePrefabAtPosition("IceWave", gameObject:GetTransform().position)
	lhWave.tag = "LHWave"
	local rhWave = CreatePrefabAtPosition("IceWave", gameObject:GetTransform().position)
	rhWave.tag = "RHWave"
	
	-- Calculate right vectors
	local up = vec3.new(0.0, 1.0, 0.0)
	local playerTransform = gameObject:GetTransform()
	local front = playerTransform:forward()
	local right = VecNormalize(VecCross(front, up))
	
	-- Set starting positions
	local body = lhWave:GetBody()
	body.position = body.position - right
	body = rhWave:GetBody()
	body.position = body.position + right	
	
	local transform = lhWave:GetTransform()
	transform.position = body.position
	transform = rhWave:GetTransform()
	transform.position = body.position
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		CreateIceWave()
		attack.shouldAttack = false
	end
end

CheckValidAttack()