function Charge(currentTime, startTime, endTime)
    local material = gameObject:GetMaterial()
    local redChanel = 5*(currentTime - startTime) / (endTime - startTime)
    material.diffuse = vec3.new(redChanel,0,0)
end

function Cooldown(currentTime, startTime, endTime)
    local material = gameObject:GetMaterial()
    local redChanel = 1 - (currentTime - startTime) / (endTime - startTime)
    material.diffuse = vec3.new(redChanel,0,0)
end

function Update()
    local health = gameObject:GetHealth()

    if(health.currentHitReactionTime >= health.hitReactionTime) then
        health.currentHitReactionTime = 0
    elseif (health.currentHitReactionTime > 0.5 * health.hitReactionTime) then
        Cooldown(health.currentHitReactionTime, health.hitReactionTime / 2, health.hitReactionTime)
    else
        Charge(health.currentHitReactionTime, 0, health.hitReactionTime / 2)
    end

end

Update()