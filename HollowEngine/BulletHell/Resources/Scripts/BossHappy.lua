function Move()
enemyBody.velocity = vec3.new(0, 0, 0)
end

function FirePattern ()
	gameObjectPath = "Resources/Json data/Bullet.json"
	local numObj = 10
	for i=0,numObj do
		go = CreateGameObject(gameObjectPath)
		body = go:GetBody()
		body.position = attackPosition
		local theta = (i/numObj * math.pi)
		local attackSpeed = 4.0
		body.velocity = vec3.new(attackSpeed*math.cos(theta), 0.0, attackSpeed*math.sin(theta))
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