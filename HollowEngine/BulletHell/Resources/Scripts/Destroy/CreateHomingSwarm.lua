function CreateHomingSwarm ()
	local offset = 0
	local numObj = 20
	local bulletPrefabPath = "Resources/Json data/BossWaterBullet.json"
	for i=0,numObj do
		local bullet = CreateGameObject(bulletPrefabPath)
	    local bulletTransform = bullet:GetTransform()
		local bulletBody = bullet:GetBody()
		local theta = (i/numObj * math.pi * 2) + math.rad(offset)
		local attackSpeed = 14.0
		bulletBody.velocity = vec3.new(attackSpeed*math.cos(theta), 0.0, attackSpeed*math.sin(theta))
		bulletTransform.position = bulletBody.position
	end
end

CreateHomingSwarm()