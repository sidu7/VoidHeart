-- Create global player accessible anywhere in Lua
player = CreatePrefabAtPosition("Player", vec3.new(0.0,0.0,0.0))

-- Create global camera, and UI camera accessible anywhere in Lua
camera = CreateGameObject("Resources/Json data/Camera.json")
UICamera = CreateGameObject("Resources/Json data/UICamera.json")

-- Create global light for shadows
globalLight = CreateGameObject("Resources/Json data/Light.json")