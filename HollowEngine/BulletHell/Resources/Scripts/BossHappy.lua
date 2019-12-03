function Move()
enemyBody.velocity = vec3.new(0, 0, 0)
end

function SemiCircle(offset)
	local gameObjectPath = "Resources/Json data/Bullet.json"
	local numObj = 20
	for i=0,numObj do
		go = CreateGameObject(gameObjectPath)
		body = go:GetBody()
		body.position = attackPosition
		local theta = (i/numObj * math.pi * 2) + math.rad(offset)
		local attackSpeed = 14.0
		body.velocity = vec3.new(attackSpeed*math.cos(theta), 0.0, attackSpeed*math.sin(theta))
		transform = go:GetTransform()
		transform.position = body.position
	end
end

function FirePattern ()
	if attackFlag then
		SemiCircle(0)
	else
		SemiCircle(5)
	end
end

function Attack()
	if currentAttackTime > baseAttackTime then
		FirePattern()
		attackFlag = not attackFlag
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