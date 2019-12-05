--[[
function FirePattern1 ()
	gameObjectPath = "Resources/Json data/Bullet.json"
	go = CreateGameObject(gameObjectPath)
	body = go:GetBody()
	body.position = attackPosition + vec3.new(0.0, 0.0, 1.0)
	body.velocity = vec3.new(0.0, 0.0, 20.0)
	-- Spawn objects in some pattern

	go1 = CreateGameObject(gameObjectPath)
	body1 = go1:GetBody()
	body1.position = attackPosition + vec3.new(1.0, 0.0, 1.0)
	body1.velocity = vec3.new(14.0, 0.0, 14.0)

	go2 = CreateGameObject(gameObjectPath)
	body2 = go2:GetBody()
	body2.position = attackPosition + vec3.new(-1.0, 0.0, 1.0)
	body2.velocity = vec3.new(-14.0, 0.0, 14.0)
end

function FirePattern2 ()
	gameObjectPath = "Resources/Json data/Bullet.json"
	local numObj = 0
	for i=1,numObj do
		go = CreateGameObject(gameObjectPath)
		body = go:GetBody()
		body.position = attackPosition + vec3.new(i*0.1, 0.0, 0.3*i+0.8)
		body.velocity = vec3.new(0.0, 0.0, 20.0)
	end
end
	

FirePattern2()
--]]
