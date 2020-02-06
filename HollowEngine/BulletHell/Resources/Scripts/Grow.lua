function grow()
	local transform = gameObject:GetTransform()
	transform.scale = transform.scale * 1.02
end

grow()