function UpdateUIScale()
	-----------------------------------------
    -- playtesting vars
	local scaleMax = vec2.new(228.0, 228.0)
	-----------------------------------------
	
	local UITransform = gameObject:GetUITransform()
	-- Set scale based on combined spell cooldown
	local combinedSpell = player:GetMagic().combinedSpell
	if combinedSpell then
	local combinedCooldown = combinedSpell.combinedCooldown
	local cooldown = combinedSpell.cooldown
	local scale = combinedCooldown / cooldown
	UITransform.scale = scale * scaleMax
	
	-- Clamp UI scale
	UITransform.scale.x = math.min(UITransform.scale.x, scaleMax.x)
	UITransform.scale.y = math.min(UITransform.scale.y, scaleMax.y)
	end
end

UpdateUIScale()