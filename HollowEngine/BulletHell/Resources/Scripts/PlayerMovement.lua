local impulse = vec3.new()
local direction = vec3.new();

local front = vec3.new(0.0, 0.0, -1.0)
local right = vec3.new(1.0, 0.0, 0.0)

front.y = 0.0
right.y = 0.0

local speed = vec3.new(1000.0, 0.0, 1000.0)

local transform = gameObject:GetTransform()
if IsKeyPressed("W") or GetAxis(CONTROLLER["LY"]) < -16000 then
	impulse = impulse + front;
	direction = direction + vec3.new(0.0, 0.0, -1.0)
end
if IsKeyPressed("S") or GetAxis(CONTROLLER["LY"]) > 16000 then
	impulse = impulse - front;
	direction = direction + vec3.new(0.0, 0.0, 1.0)
end
if IsKeyPressed("A") or GetAxis(CONTROLLER["LX"]) < -16000 then
	impulse = impulse - right;
	direction = direction + vec3.new(-1.0, 0.0, 0.0)
end
if IsKeyPressed("D") or GetAxis(CONTROLLER["LX"]) > 16000 then
	impulse = impulse + right;
	direction = direction + vec3.new(1.0, 0.0, 0.0)
end
if IsKeyPressed("SPACE") or IsControllerButtonTriggered(CONTROLLER["A"]) then
	impulse = impulse + vec3.new(0.0, 20.0, 0.0);
end

-- look direction
local xVelocity = GetAxis(CONTROLLER["RX"])
local zVelocity = GetAxis(CONTROLLER["RY"])
local totalVelocity = math.sqrt(xVelocity*xVelocity + zVelocity*zVelocity)
local xVelocityNorm = xVelocity / totalVelocity
local zVelocityNorm = zVelocity / totalVelocity

local rot = vec3.new(0.0, 0.0, 0.0)
local tangent = xVelocityNorm / zVelocityNorm
local radians = math.atan(tangent)
local degree = radians * 180 / math.pi
if zVelocityNorm >= 0 then  
	rot = vec3.new(0.0, degree, 0.0)
    transform:Rotate(rot)
end
if zVelocityNorm < 0 then 
	rot = vec3.new(0.0, degree + 180, 0.0)
    transform:Rotate(rot)
end

-- cardinal direction look
--[[local rot = vec3.new(0.0, 0.0, 0.0)
if GetAxis(CONTROLLER["RY"]) < -16000 then --up
	rot = vec3.new(0.0, 180.0, 0.0)
end
if GetAxis(CONTROLLER["RY"]) > 16000 then  --down
	rot = vec3.new(0.0, 0.0, 0.0)
end
if GetAxis(CONTROLLER["RX"]) < -16000 then --left
	rot = vec3.new(0.0, 270.0, 0.0)
end
if GetAxis(CONTROLLER["RX"]) > 16000 then  --right
	rot = vec3.new(0.0, 90.0, 0.0)
end
local transform = gameObject:GetTransform()
transform:Rotate(rot) --]]

impulse = impulse * speed;

ApplyLinearImpulse(gameObject, impulse)

local body = gameObject:GetBody()

-- Damp Overall Velocity and Rotation
body.velocity = body.velocity - 0.8 * body.velocity 
body.angularVelocity = body.angularVelocity - 0.8* body.angularVelocity;
