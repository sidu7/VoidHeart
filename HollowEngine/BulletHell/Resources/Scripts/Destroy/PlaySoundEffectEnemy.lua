function PlaySoundEffect ()
	-- Play SFX based on object tag
	local tag = gameObject.tag
	local audioPath = ""
	if tag == "ECircleLookdir" then
		audioPath = 
		"Resources/Audio/SFX/EnemyDeathCircleLookdir.wav"
	elseif tag == "EFollowLookdir" then
		audioPath = 
		"Resources/Audio/SFX/EnemyDeathFollowLookdir.wav"
	elseif tag == "ETurretTarget" then
		audioPath = 
		"Resources/Audio/SFX/EnemyDeathTurretTarget.wav"
	elseif tag == "ETurretdiagonal" then
		audioPath = 
		"Resources/Audio/SFX/EnemyDeathTurretdiagonal.wav"
	elseif tag == "ETurretCardinal" then
		audioPath = 
		"Resources/Audio/SFX/EnemyDeathTurretCardinal.wav"
	elseif tag == "ERunsawayLookdir" then
	audioPath = 
		"Resources/Audio/SFX/EnemyDeathRunsawayLookdir.wav"
	else
		audioPath = ""
	end
	PlaySFX(audioPath)
end

PlaySoundEffect()