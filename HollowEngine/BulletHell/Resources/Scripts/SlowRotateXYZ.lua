local transform = gameObject:GetTransform()
local rot = transform.rotation
transform:Rotate(vec3.new(rot.x+1.0, rot.y+1.0, rot.z+2.0))
