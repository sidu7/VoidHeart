function PlaySoundEffect ()
	-- Play SFX based on object tag
	local tag = gameObject.tag
	local audioPath = ""
	if tag == "FIREFIRE" then
		audioPath = "Resources/Audio/SFX/FireFireSpellDestroy.wav"
	elseif tag == "WATERWATER" then
		audioPath = 
		"Resources/Audio/SFX/WaterWaterSpellDestroy.wav"
	else
		audioPath = ""
	end
	PlaySFX(audioPath)
end

PlaySoundEffect()