function Move()
	local speed = 30.0
	if math.abs(enemyBody.velocity.x) < speed then
		enemyBody.velocity.x = speed * enemyBody.velocity.x / math.abs(enemyBody.velocity.x)
	end
	-- Turn around if past a certain x point
	local reversePosition = 10.0
	if 	enemyBody.position.x > reversePosition or
		enemyBody.position.x < -reversePosition then
		enemyBody.velocity.x = -enemyBody.velocity.x
	end
end

function FirePattern ()
	gameObjectPath = "Resources/Json data/Bullet.json"
	local numObj = 10
	for i=1,numObj do
		go = CreateGameObject(gameObjectPath)
		body = go:GetBody()
		body.position = attackPosition + vec3.new(i*0.1, 0.0, 0.3*i+0.8)
		body.velocity = vec3.new(0.0, 0.0, 20.0)
		transform = go:GetTransform()
		transform.position = body.position
	end
end

function Attack()
	if currentAttackTime > baseAttackTime then
		FirePattern()
		currentAttackTime = 0.0
	end
end

function Update()
	Move()
	Attack()
end

Update()