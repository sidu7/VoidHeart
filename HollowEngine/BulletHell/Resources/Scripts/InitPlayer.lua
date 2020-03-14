player = CreatePrefabAtPosition("Player", vec3.new(0.0,0.0,0.0))
local gameObjectPath = "Resources/Json data/UICamera.json"
UICamera = CreateGameObject(gameObjectPath)
local gameObjectPath = "Resources/Json data/Camera.json"
camera = CreateGameObject(gameObjectPath)
local gameObjectPath = "Resources/Json data/Light.json"
globalLight = CreateGameObject(gameObjectPath)