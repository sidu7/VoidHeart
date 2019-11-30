function Move()
enemyBody.velocity = enemyBody.velocity + vec3.new(-1.0, 0.0, 0.0)
enemyBody.velocity = 0.8 * enemyBody.velocity
end

function FirePattern1 ()
	gameObjectPath = "Resources/Json data/Bullet.json"
	local numObj = 10
	for i=1,numObj do
		go = CreateGameObject(gameObjectPath)
		body = go:GetBody()
		body.position = attackPosition + vec3.new(i*0.1, 0.0, 0.3*i+0.8)
		body.velocity = vec3.new(0.0, 0.0, 20.0)
	end
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