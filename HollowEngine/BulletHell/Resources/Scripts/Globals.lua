-- Globals
currentFloor = 0
currentRoom = 4

-- Dungeon Configuration
firstFloorRoomCount = 1
numFloors = 1
dungeonLength = 1
dungeonBreadth = 1

-- main menu Room Configuration
roomSize = 20
wallLength = 8
wallHeight = 5
wallThickness = 1
doorWidth = 3
doorHeight = 2
doorThickness = 3

-- Enemy
firstFloorEnemyCount = 0

-- Cheat codes
gameMuted = false

-- Utils
function GetRoomCenterPosition()
	local coords = GetDungeonFloor(currentFloor):GetEntrance():GetCoords()
	local pos = vec3.new(coords.y * roomSize + roomSize/2, 1, coords.x * roomSize + roomSize/2)
	return pos
end

-- RandomGlobals
waterBossZOffset = 0
waterBossTheta = 0
waterBossCircle = 1
waterBossCircleCenter = vec3.new(0,0,0)
waterBossCircleSelected = true
waterBossMoveDirection = true