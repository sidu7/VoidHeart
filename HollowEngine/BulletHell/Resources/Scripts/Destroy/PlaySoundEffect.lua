function PlaySoundEffect ()
	-- Play SFX based on object tag
	local tag = gameObject.tag
	local audioPath = ""
	if tag == "FIREFIRE" then
		audioPath = "Resources/Audio/SFX/FireFireSpellDestroy.wav"
	elseif tag == "WATERWATER" then
		audioPath = 
		"Resources/Audio/SFX/WaterWaterSpellDestroy.wav"
	elseif tag == "FIREWATER" then
		audioPath = 
		"Resources/Audio/SFX/FireWaterSpellDestroy.wav"
	elseif tag == "LHWave" then
		-- Only want this sound once per cast of AIRWATER
		audioPath = 
		"Resources/Audio/SFX/AirWaterSpellDestroy.wav"
	elseif tag == "EARTHEARTH" then
		audioPath = 
		"Resources/Audio/SFX/EarthEarthSpellDestroy.wav"
	else
		audioPath = ""
	end
	PlaySFX(audioPath)
end

PlaySoundEffect()