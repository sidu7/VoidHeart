impulse = vec3.new()

if isMoveForward then
	impulse.z = 1;
end
if isMoveBackward then
	impulse.z = -1;	
end
if isStrafeLeft then
	impulse.x = -1;
end
if isStrafeRight then
	impulse.x = 1;
end
