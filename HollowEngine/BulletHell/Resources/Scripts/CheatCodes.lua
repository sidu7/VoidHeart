-- Put your cool cheat codes here!

function MuteAudio()
	if IsKeyTriggered("M") then
		gameMuted = not gameMuted
		Mute(gameMuted)
	end
end

function CheckCheatCodes()
	MuteAudio()
end

CheckCheatCodes()