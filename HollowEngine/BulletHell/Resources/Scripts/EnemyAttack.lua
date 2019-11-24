gameObjectPath = "Resources/Json data/Barrier.json"
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