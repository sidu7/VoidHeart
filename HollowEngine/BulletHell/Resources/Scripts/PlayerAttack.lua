function FirePattern1 ()
	gameObjectPath = "Resources/Json data/PlayerBullet.json"
	go = CreateGameObject(gameObjectPath)
	body = go:GetBody()
	body.position = attackPosition + vec3.new(0.0, 0.0, -2.0)
	body.velocity = vec3.new(0.0, 0.0, -20.0)
	transform = go:GetTransform()
	transform.position = body.position
end

FirePattern1()
