function SpellCollected ()
	-- Make some effect and sound based on spell collected
	local spellType = gameObject:GetSpell().spellType
	local audioPath = ""
	if spellType == 1 then
		audioPath = "Resources/Audio/SFX/FireSpellCollect.wav"
	elseif spellType == 2 then
		audioPath = "Resources/Audio/SFX/AirSpellCollect.wav"
	elseif spellType == 4 then
		audioPath = "Resources/Audio/SFX/EarthSpellCollect.wav"
	else
		audioPath = "Resources/Audio/SFX/WaterSpellCollect.wav"
	end
	PlaySFX(audioPath)
end

SpellCollected()