function RunSlideShow()
	local slideSwitchTime = 3

	slideShowTimer = slideShowTimer + GetFrameTime()

	if slideShowTimer > slideSwitchTime then
		local path = CreditsPaths[slideNum]
		gameObject:GetUIImage():SetTexture(path)
		slideShowTimer = slideShowTimer - slideSwitchTime

		if slideNum == 7 then
			slideNum = 0
		end
		
		slideNum = slideNum + 1
	end
end

RunSlideShow()