function CreateFireballs ()
	local offset = 0
	local numObj = 10
	for i=0,numObj do
		local go = CreatePrefabAtPosition("PlayerFire", gameObject:GetTransform().position)
		go:GetDestroyTimer().maxTime = 0.2
		local body = go:GetBody()
		local theta = (i/numObj * math.pi * 2) + math.rad(offset)
		local attackSpeed = 14.0
		body.velocity = vec3.new(attackSpeed*math.cos(theta), 0.0, attackSpeed*math.sin(theta))
		local transform = go:GetTransform()
		transform.position = body.position
	end
	PlaySFX("Resources/Audio/SFX/FireEarthSpellDestroy.wav")
end

CreateFireballs()