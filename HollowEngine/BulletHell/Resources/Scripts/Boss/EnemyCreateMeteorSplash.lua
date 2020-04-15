function CreateFireballs ()
	local offset = 0
	local numObj = 10
	for i=0,numObj do
        -- Create bullet
	    local bulletPrefabPath = "Resources/Json data/Bullet.json"
	    local bullet = CreateGameObject(bulletPrefabPath)
        local bulletTransform = bullet:GetTransform()
	    local bulletBody = bullet:GetBody()
   
        -- Setting position
        bulletBody.position = gameObject:GetTransform().position
        bulletTransform.position = gameObject:GetTransform().position

		bullet:GetDestroyTimer().maxTime = 0.2
		local theta = (i/numObj * math.pi * 2) + math.rad(offset)
		local attackSpeed = 14.0
		bulletBody.velocity = vec3.new(attackSpeed*math.cos(theta), 0.0, attackSpeed*math.sin(theta))
		bulletTransform.position = bulletBody.position
	end
	PlaySFX("Resources/Audio/SFX/FireEarthSpellDestroy.wav")
end

CreateFireballs()