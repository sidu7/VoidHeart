function ShootInDirection(dirX, dirY, dirZ)
    -----------------------------------------
    -- playtesting vars
	local bulletSpeed = 20.0
    -----------------------------------------
    local transform = gameObject:GetTransform()
	local spawnPos = transform.position

	local bulletPrefabPath = "Resources/Json data/Bullet.json"
	local bullet = CreateGameObject(bulletPrefabPath)
    local bulletTransform = bullet:GetTransform()
	local bulletBody = bullet:GetBody()
    
    -- Setting position
    bulletBody.position = spawnPos
    bulletTransform.position = spawnPos

    -- Setting velocity
    bulletBody.velocity = bulletSpeed * vec3.new(dirX, dirY, dirZ)
end

function Shoot()    	
    ShootInDirection(-1, 0, 0)
    ShootInDirection(1, 0, 0)
    ShootInDirection(0, 0, -1)
    ShootInDirection(0, 0, 1)
end

function Update()
	local attack = gameObject:GetAttack()
	if attack.currentAttackTime > attack.baseAttackTime then
	    Shoot()
        attack.currentAttackTime = 0
    end
end

Update()