function ShootInDirection(dirX, dirY, dirZ)
    -----------------------------------------
    -- playtesting vars
	local bulletSpeed = 10.0
    -----------------------------------------
    local transform = gameObject:GetTransform()
	local spawnPos = transform.position

	local bulletPrefabPath = "Resources/Json data/Bullet.json"
	local bullet = CreateGameObject(bulletPrefabPath)
    local bulletTransform = bullet:GetTransform()
	local bulletBody = bullet:GetBody()
    bullet:GetDestroyTimer().maxTime = 0.3
    
    -- Setting position
    bulletBody.position = spawnPos
    bulletTransform.position = spawnPos

    -- Setting velocity
    local direction = vec3.new(dirX, dirY, dirZ)
    local length = dirX * dirX + dirY * dirY + dirZ * dirZ
    direction = direction * (1 / length);
    bulletBody.velocity = bulletSpeed * direction
    bulletBody.useGravity = true
end

function ShootInAllDirections()
    
	local offset = 0
    local numBullets = 20
    for i = 0, numBullets do
	    local theta = (i / numBullets * math.pi * 2) + math.rad(offset)
        ShootInDirection(math.cos(theta), 0.5, math.sin(theta))
    end
	PlaySFX("Resources/Audio/SFX/FireFireSpellDestroy.wav")
end

local function CreateFireballs ()
	ShootInAllDirections()
end

CreateFireballs()