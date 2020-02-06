function Shoot()
    local transform = gameObject:GetTransform()
	local spawnPos = transform.position

    local target = player
    local targetTransform = target:GetTransform()
    local targetPos = targetTransform.position
    
    local xDir = targetPos.x - spawnPos.x
	local zDir = targetPos.z - spawnPos.z
	local dirLength = math.sqrt(xDir*xDir + zDir*zDir)
	local xDirNorm = xDir / dirLength
	local zDirNorm = zDir / dirLength
	
    -- look at the target
    local rot = vec3.new(0.0, 0.0, 0.0)
    local tangent = xDir / zDir
    local radians = math.atan(tangent)
    local degree = radians * 180 / math.pi
    if xDirNorm >= 0 then  
	    rot = vec3.new(0.0, degree, 0.0)
        transform:Rotate(rot)
    end
    if zDirNorm < 0 then 
	    rot = vec3.new(0.0, degree + 180, 0.0)
        transform:Rotate(rot)
    end

    
	local gameObjectPath = "Resources/Json data/Bullet.json"
	--local go = CreateGameObject(gameObjectPath)
	--body.velocity = attackSpeed * vec3.new(xVelocityNorm, 0.0, zVelocityNorm)

	local attackSpeed = 10.0
    
end

function Update()
    
	Shoot()
end

Update()