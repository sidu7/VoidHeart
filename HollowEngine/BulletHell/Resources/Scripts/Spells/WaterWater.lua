function CreateBubbleShield()
	-----------------------------------------
    -- playtesting vars
	-----------------------------------------
	local go = CreatePrefabAtPosition("PlayerBubble", gameObject:GetTransform().position)
	local body = go:GetBody()
	
	--PlaySFX("Resources/Audio/SFX/PlayerAttack.wav")
end

function CheckValidAttack()
	local attack = gameObject:GetAttack()
	if attack.shouldAttack then
		CreateBubbleShield()
		attack.shouldAttack = false
	end
end

CheckValidAttack()