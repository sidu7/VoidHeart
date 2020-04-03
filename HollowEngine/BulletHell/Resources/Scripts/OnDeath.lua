function Update()
    local health = player:GetHealth()
    local hitPoints = health.hitPoints
    if (hitPoints < 0) then
        if (health.isDying ~= true) then -- it is executed only once
            health.isDying = true
	        local playerTransform = player:GetTransform()
            local playerPos = vec3.new(playerTransform.position.x, playerTransform.position.y, playerTransform.position.z)

	        local offset = 0
	        local numObj = 20
	        for i=0,numObj do
		        local go = CreatePrefabAtPosition("PlayerFire", gameObject:GetTransform().position)
		        local body = go:GetBody()
		        local theta = (i/numObj * math.pi * 2) + math.rad(offset)
		        local attackSpeed = 14.0
		        body.velocity = vec3.new(attackSpeed*math.cos(theta), 0.0, attackSpeed*math.sin(theta))
		        local transform = go:GetTransform()
		        transform.position = body.position

                go:GetDestroyTimer().maxTime = 0.3
	        end
            
            if (gameObject.tag == "PLAYER") then 
                PlaySFX("Resources/Audio/SFX/lose.wav")
            end
        end
        local body = gameObject:GetBody()
        body.velocity = vec3.new(0.0, 0.0, 0.0)
    end
end

Update()
