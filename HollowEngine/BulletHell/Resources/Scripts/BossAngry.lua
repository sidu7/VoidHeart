function Move()
	local speed = 15.0
	if math.abs(enemyBody.velocity.x) < 0.001 then
		enemyBody.velocity.z = speed * enemyBody.velocity.z / math.abs(enemyBody.velocity.z)
		elseif math.abs(enemyBody.velocity.z) < 0.001 then
		enemyBody.velocity.x = speed * enemyBody.velocity.x / math.abs(enemyBody.velocity.x)
	end
	-- Turn around if past a certain x point
	local reversePositionX = 20.0
	local reversePositionZ = 20.0

	if 	enemyBody.position.x > reversePositionX or
		enemyBody.position.x < -reversePositionX then
		enemyBody.position.x = reversePositionX * enemyBody.velocity.x / math.abs(enemyBody.velocity.x) ;
		enemyBody.velocity.z = -enemyBody.velocity.x
		enemyBody.velocity.x = 0.0
	elseif 	enemyBody.position.z > reversePositionZ or
		enemyBody.position.z < -reversePositionZ then
		enemyBody.position.z =reversePositionZ * enemyBody.velocity.z / math.abs(enemyBody.velocity.z)
		enemyBody.velocity.x = enemyBody.velocity.z
		enemyBody.velocity.z = 0.0 
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
	local gameObjectPath = "Resources/Json data/Bullet.json"
	local numObj = 5
	for i=1,numObj do
		go = CreateGameObject(gameObjectPath)
		body = go:GetBody()
		body.position = attackPosition + vec3.new(0, 0.0, 0.0)
		body.velocity = vec3.new((i - numObj/2)*1.0, 0.0, zvel)
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

function FirePattern ()
	FireAtPlayer()
	for i=0,10 do 
	FireFollowBullet()
	end
	PlaySFX("Resources/Audio/SFX/1UP.wav")
end

function Attack()
	if currentAttackTime > baseAttackTime then
		FirePattern()
		currentAttackTime = 0.0
	end
end

function TransitionMove()
	local transitionspeed = -10.0
	enemyBody.velocity.z = transitionspeed
end
function Update()
	Move()
	if transitionTime < 0.0001 then
		Attack()
	end
end

Update()