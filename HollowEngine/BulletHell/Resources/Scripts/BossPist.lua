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

function FirePattern (num)
	if(num < 4.0) then
	FireAtPlayer()
	elseif (num < 7.0 ) then
	VBulletPattern(10.0)
	elseif (num < 10.0) then
	SemiCircle(0)
	else
	FireAtPlayer()
	VBulletPattern(10.0)
	SemiCircle(0)
	end
end

function Attack(num)
	if currentAttackTime > baseAttackTime then
		FirePattern(num)
		currentAttackTime = 0.0
	end
end

function Update()
	Move()
	if transitionTime < 0.0001 then
		Attack(math.random(10))
	end
end

Update()