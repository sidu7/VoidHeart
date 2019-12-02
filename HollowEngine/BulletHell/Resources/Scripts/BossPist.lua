function Move()
	local speed = 20.0
	if math.abs(enemyBody.velocity.x) < speed then
		enemyBody.velocity.x = speed * enemyBody.velocity.x / math.abs(enemyBody.velocity.x)
	end
	-- Turn around if past a certain x point
	local reversePosition = 30.0
	if 	enemyBody.position.x > reversePosition or
		enemyBody.position.x < -reversePosition then
		enemyBody.velocity.x = -enemyBody.velocity.x
	end
end

function FireAtPlayer()
	gameObjectPath = "Resources/Json data/Shuriken.json"

	go = CreateGameObject(gameObjectPath)
	body = go:GetBody()
	body.position = attackPosition
	
	local xVelocity = playerBody.x - body.position.x
	local zVelocity = playerBody.z- body.position.z
	local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
	xVelocity = xVelocity / totalVelocity
	zVelocity = zVelocity / totalVelocity
	
	local attackSpeed = 40.0
	body.velocity = attackSpeed * vec3.new(xVelocity, 0.0, zVelocity)
	
	transform = go:GetTransform()
	transform.position = body.position
end

function VBulletPattern(zvel)
	gameObjectPath = "Resources/Json data/Bullet.json"
	local numObj = 10
	for i=1,numObj do
		go = CreateGameObject(gameObjectPath)
		body = go:GetBody()
		body.position = attackPosition + vec3.new(0, 0.0, 0.0)
		body.velocity = vec3.new((i - numObj/2)*3.0, 0.0, zvel)
		transform = go:GetTransform()
		transform.position = body.position
	end
end

function SemiCircle(offset)
	local gameObjectPath = "Resources/Json data/Bullet.json"
	local numObj = 10
	for i=0,numObj do
		go = CreateGameObject(gameObjectPath)
		body = go:GetBody()
		body.position = attackPosition
		local theta = (i/numObj * math.pi) + math.rad(offset)
		local attackSpeed = 24.0
		body.velocity = vec3.new(attackSpeed*math.cos(theta), 0.0, attackSpeed*math.sin(theta))
		transform = go:GetTransform()
		transform.position = body.position
	end
end

function FirePattern ()
	FireAtPlayer()
	VBulletPattern(10.0)
	SemiCircle(0)
end

function Attack()
	if currentAttackTime > baseAttackTime then
		FirePattern()
		currentAttackTime = 0.0
	end
end

function Update()
	Move()
	if transitionTime < 0.0001 then
		Attack()
	end
end

Update()