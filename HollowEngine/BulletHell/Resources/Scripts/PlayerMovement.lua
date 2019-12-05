impulse = vec3.new()
direction = vec3.new();

front = vec3.new(0.0, 0.0, -1.0)
right = vec3.new(1.0, 0.0, 0.0)

front.y = 0.0
right.y = 0.0

speed = vec3.new(200.0, 0.0, 200.0)

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
	dashSpeed = 15.0
	impulse = impulse + direction * dashSpeed
	PlaySFX("Resources/Audio/SFX/PlayerDash.wav")
end

impulse = impulse * speed;

-- Damp Overall Velocity and Rotation
body.velocity = body.velocity - 0.8 * body.velocity 
