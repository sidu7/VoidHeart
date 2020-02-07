function CreateFireballs ()
	local offset = 0
	local gameObjectPath = "Resources/Json data/PlayerBullet.json"
	local numObj = 20
	for i=0,numObj do
		local go = CreateGameObject(gameObjectPath)
		local body = go:GetBody()
		body.position = gameObject:GetTransform().position
		local theta = (i/numObj * math.pi * 2) + math.rad(offset)
		local attackSpeed = 14.0
		body.velocity = vec3.new(attackSpeed*math.cos(theta), 0.0, attackSpeed*math.sin(theta))
		local transform = go:GetTransform()
		transform.position = body.position
	end
end

CreateFireballs()