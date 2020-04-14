function PlaySoundEffect ()
	-- Play SFX based on object tag
	local tag = gameObject.tag
	local audioPath = ""
	if tag == "HPPICKUP" then
		audioPath = 
		"Resources/Audio/SFX/HPPickup.wav"
	elseif tag == "SPEEDPICKUP" then
		audioPath = 
		"Resources/Audio/SFX/SpeedPickup.wav"
	elseif tag == "INVINCIBLEPICKUP" then
		audioPath = 
		"Resources/Audio/SFX/InvinciblePickup.wav"
	elseif tag == "DAMAGEPICKUP" then
		audioPath = 
		"Resources/Audio/SFX/DamagePickup.wav"
	elseif tag == "RATEOFFIREPICKUP" then
		audioPath = 
		"Resources/Audio/SFX/RateOfFirePickup.wav"
	else
		audioPath = ""
	end
	PlaySFX(audioPath)
end

PlaySoundEffect()