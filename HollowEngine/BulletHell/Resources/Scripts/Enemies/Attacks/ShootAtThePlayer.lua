function Shoot()
    local transform = gameObject:GetTransform()
	attackPosition = transform.position

    local target = player
    local targetTransform = target:GetTransform()
    local targetPos = transformPlayer.position
    
	body = go:GetBody()
	body.position = attackPosition
    
    local xVelocity = targetPos.x - body.position.x
	local zVelocity = targetPos.z - body.position.z
	local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
	local xVelocityNorm = xVelocity / totalVelocity
	local zVelocityNorm = zVelocity / totalVelocity
	
	local attackSpeed = 10.0
	body.velocity = attackSpeed * vec3.new(xVelocityNorm, 0.0, zVelocityNorm)

    -- look at the target
    local rot = vec3.new(0.0, 0.0, 0.0)
    local tangent = xVelocityNorm / zVelocityNorm
    local radians = math.atan(tangent)
    local degree = radians * 180 / math.pi
    if zVelocityNorm >= 0 then  
	    rot = vec3.new(0.0, degree, 0.0)
        transform:Rotate(rot)
    end
    if zVelocityNorm < 0 then 
	    rot = vec3.new(0.0, degree + 180, 0.0)
        transform:Rotate(rot)
    end

    
	local gameObjectPath = "Resources/Json data/PlayerBullet.json"
	go = CreateGameObject(gameObjectPath)

    
end

function Update()
	Shoot()
end

Update()