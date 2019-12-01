#pragma once
#include "Hollow/Systems/System.h"
class BlockSystem;
struct Tetromino;

class LayerSystem : public Hollow::System
{
	REGISTERSYSTEM(LayerSystem, 2);
public:
	~LayerSystem() {}
	void Init() override;
	void Update() override;
	void AddGameObject(Hollow::GameObject* object) override;

public:
	bool mLayers[15][10][10]; // height, z, x
	bool mFloor[15][10][10]; // for blocks that are fixed

	BlockSystem* mBlockSystem;
	Tetromino* mActiveTetromino;

	float mMoveInterval;
	float mTimePast;
	
	glm::vec3 mActiveTetrominoPosition;
};
