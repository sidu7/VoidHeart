function ShootInDirection(dirX, dirY, dirZ)
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
	local bulletSpeed = 15.0
    bulletBody.velocity = bulletSpeed * vec3.new(dirX, dirY, dirZ)
end

function Shoot()    	
    local transform = gameObject:GetTransform()
    local shootDir = transform:forward()

    ShootInDirection(shootDir.x, shootDir.y, shootDir.z)
end

function Update()
	local attack = gameObject:GetAttack()
	if attack.currentAttackTime > attack.baseAttackTime then
	    Shoot()
        attack.currentAttackTime = 0
    end
end

Update()