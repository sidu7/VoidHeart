function SpellCollected ()
	-- Make some effect and sound based on spell collected
	local spellType = gameObject:GetSpell().spellType
	if spellType == 1 then
	
	end
	PlaySFX("Resources/Audio/SFX/PlayerAttack.wav")
end

SpellCollected()