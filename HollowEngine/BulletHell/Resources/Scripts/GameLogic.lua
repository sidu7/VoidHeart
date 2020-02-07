function CheckRoomBounds(playerPos, coords)
	if playerPos.x > coords.y * roomSize 
	 and playerPos.x < coords.y * roomSize + 1
	 and playerPos.z > coords.x * roomSize 
	 and playerPos.z < coords.x * roomSize + 1 then
		currentRoom = coords.y * dungeonLength + coords.x
	end
end

function UpdateCurrentRoom( )
	local playerPosition = player:GetBody().position

	-- check position against adjacent rooms
	local room = GetDungeonFloor(currentFloor):GetRoomFromIndex(currentRoom)
	local roomCoords = room:GetCoords()
	local doors = room:GetDoorBits()

	if doors & 1 then
		CheckRoomBounds(playerPosition, ivec2.new(roomCoords.x, roomCoords.y - 1))
	end

	if doors & 2 then
		CheckRoomBounds(playerPosition, ivec2.new(roomCoords.x + 1, roomCoords.y))
	end

	if doors & 4 then
		CheckRoomBounds(playerPosition, ivec2.new(roomCoords.x, roomCoords.y + 1))
	end

	if doors & 8 then
		CheckRoomBounds(playerPosition, ivec2.new(roomCoords.x - 1, roomCoords.y))
	end
end

UpdateCurrentRoom()