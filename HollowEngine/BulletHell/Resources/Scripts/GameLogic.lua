function CheckRoomBounds(playerPos, coords)
	if (playerPos.x > coords.y * roomSize 
	 and playerPos.x < (coords.y + 1) * roomSize
	 and playerPos.z > coords.x * roomSize 
	 and playerPos.z < (coords.x + 1) * roomSize) then
		currentRoom = coords.x * dungeonLength + coords.y
	end
	--print("room ", coords.x, coords.y)
end

function UpdateCurrentRoom( )
	local playerPosition = player:GetBody().position

	-- check position against adjacent rooms
	local room = GetDungeonFloor(currentFloor):GetRoomFromIndex(currentRoom)
	local roomCoords = room:GetCoords()
	local doors = room:GetDoorBits()

	--print("doors ",doors)
	if ((doors & 1) == 1) then
		CheckRoomBounds(playerPosition, ivec2.new(roomCoords.x - 1, roomCoords.y))
	end

	if ((doors & 2) == 2) then
		CheckRoomBounds(playerPosition, ivec2.new(roomCoords.x, roomCoords.y + 1))
	end

	if ((doors & 4) == 4) then
		CheckRoomBounds(playerPosition, ivec2.new(roomCoords.x + 1, roomCoords.y))
	end

	if ((doors & 8) == 8) then
		CheckRoomBounds(playerPosition, ivec2.new(roomCoords.x, roomCoords.y - 1))
	end
end

UpdateCurrentRoom()
--print("curr: ", currentRoom)
-- Lock or Unlock Room
local room = GetDungeonFloor(currentFloor):GetRoomFromIndex(currentRoom)
if room:IsCleared() then
	room:UnlockRoom()
else 
	room:LockDownRoom()
end