impulse = vec3.new()
direction = vec3.new();

front = fpsCamera.frontDirection
right = fpsCamera.rightDirection

front.y = 0.0
right.y = 0.0

if isMoveForward then
	impulse = impulse + front;
	direction = direction + vec3.new(0.0, 0.0, -1.0)
end
if isMoveBackward then
	impulse = impulse - front;
	direction = direction + vec3.new(0.0, 0.0, 1.0)
end
if isStrafeLeft then
	impulse = impulse - right;
	direction = direction + vec3.new(-1.0, 0.0, 0.0)
end
if isStrafeRight then
	impulse = impulse + right;
	direction = direction + vec3.new(1.0, 0.0, 0.0)
end
if jump then
	impulse = impulse + vec3.new(0.0, 20.0, 0.0);
end
if dash then
	dashSpeed = 40.0
	impulse = impulse + direction*dashSpeed
end

-- Damp Overall Velocity and Rotation
player.velocity = player.velocity - 0.8 * player.velocity 
player.angularVelocity = player.angularVelocity - 0.8 * player.angularVelocity 
