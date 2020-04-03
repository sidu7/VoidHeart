function CheckRoomBounds(playerPos, coords)
	if (playerPos.x > coords.y * roomSize 
	 and playerPos.x < (coords.y + 1) * roomSize
	 and playerPos.z > coords.x * roomSize 
	 and playerPos.z < (coords.x + 1) * roomSize) then
		local oldRoomIndex = currentRoom
		currentRoom = coords.x * dungeonLength + coords.y
        OnRoomEntered(currentRoom, oldRoomIndex)
	end
	-- print("Room ", coords.x, coords.y)
end

function CheckIfInFirstRoom(room)
	-- If current floor is 0
	-- And current room is entrance (5 in dungeon room enum)
	return currentFloor == 0 and room.roomType == 5
end

function UpdateCurrentRoom( )
    local playerPosition = player:GetBody().position

	-- check position against adjacent rooms
	local room = GetDungeonFloor(currentFloor):GetRoomFromIndex(currentRoom)
	local roomCoords = room:GetCoords()
	local doors = room:GetDoorBits()

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

    -- Lock or Unlock Room
	if CheckIfInFirstRoom(room) then
	-- Skip very first room on first floor, will unlock after spell pickup
	else
		if room:IsCleared() then
			room:UnlockRoom()
		else 
			room:LockDownRoom()
		end
	end
end

UpdateCurrentRoom()