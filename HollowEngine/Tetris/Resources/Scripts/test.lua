impulse = vec3.new()

front = fpsCamera.frontDirection
right = fpsCamera.rightDirection

front.y = 0.0
right.y = 0.0

if isMoveForward then
	impulse = impulse + front;
end
if isMoveBackward then
	impulse = impulse - front;	
end
if isStrafeLeft then
	impulse = impulse - right;
end
if isStrafeRight then
	impulse = impulse + right;
end
if jump then
	impulse = impulse + vec3.new(0.0, 20.0, 0.0);
end

-- Damp Overall Velocity and Rotation
player.velocity = player.velocity - 0.1 * player.velocity 
player.angularVelocity = player.angularVelocity - 0.1 * player.angularVelocity 
