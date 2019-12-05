function Move()
	local speed = 4.0
	local xVelocity = playerBody.x - enemyBody.position.x
	local zVelocity = playerBody.z - enemyBody.position.z
	local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
	xVelocity = xVelocity / totalVelocity
	zVelocity = zVelocity / totalVelocity
	
	enemyBody.velocity = speed * vec3.new(xVelocity, 0.0, zVelocity)
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
	
	local attackSpeed = 30.0
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

function FireFollowBullet()
	local gameObjectPath = "Resources/Json data/FollowBullet.json"
	go = CreateGameObject(gameObjectPath)
	body = go:GetBody()
	body.position = attackPosition
	local theta = math.random(0, 360)
	theta = theta * 3.141592 / 180.0
	local speed = 10.0
	body.velocity = vec3.new(math.cos(theta)*speed, 0.0, math.sin(theta)*speed)
	transform = go:GetTransform()
	transform.position = body.position
end

function FireHomingBullet()
local gameObjectPath = "Resources/Json data/HomingBullet.json"
	go = CreateGameObject(gameObjectPath)
	body = go:GetBody()
	body.position = attackPosition + vec3.new(math.random(-2,2),0.0,math.random(-2,2))
	local theta = math.random(0, 360)
	theta = theta * 3.141592 / 180.0
	local speed = 15.0
	body.velocity = vec3.new(math.cos(theta)*speed, 0.0, math.sin(theta)*speed)
	transform = go:GetTransform()
	transform.position = body.position
end

function FirePattern ()
	FireAtPlayer() 

	FireFollowBullet()
	FireFollowBullet()

	FireHomingBullet()
	FireHomingBullet()
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