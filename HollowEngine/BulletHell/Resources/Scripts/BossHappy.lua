function Move()
enemyBody.velocity = vec3.new(0, 0, 0)
end

function FirePattern1 ()
	gameObjectPath = "Resources/Json data/Bullet.json"
	go = CreateGameObject(gameObjectPath)
	body = go:GetBody()
	body.position = attackPosition + vec3.new(0.0, 0.0, 1.0)
	body.velocity = vec3.new(0.0, 0.0, 20.0)
	transform = go:GetTransform()
	transform.position = body.position
end

function Attack()
	if currentAttackTime > baseAttackTime then
		FirePattern1()
		currentAttackTime = 0.0
	end
end

function Update()
	Move()
	Attack()
end

Update()