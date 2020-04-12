function CreateBubbleShield()
	-----------------------------------------
    -- playtesting vars
	-----------------------------------------
	local go = CreatePrefabAtPosition("PlayerBubble", gameObject:GetTransform().position)	
	PlaySFX("Resources/Audio/SFX/WaterWaterSpellCreate.wav")
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		CreateBubbleShield()
		attack.shouldAttack = false
	end
end

CheckValidAttack()