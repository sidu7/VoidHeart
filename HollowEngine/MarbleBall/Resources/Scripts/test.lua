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
	impulse = impulse + vec3.new(0.0, 4.0, 0.0);
end

-- Damp Overall Velocity and Rotation
	--player.velocity.x = player.velocity.x - 0.02 * player.velocity.x;
	--player.velocity.z = player.velocity.z - 0.02 * player.velocity.z;
	--player.velocity.y = player.velocity.y - 0.01 * player.velocity.y;

player.angularVelocity = player.angularVelocity - 0.03 * player.angularVelocity 
