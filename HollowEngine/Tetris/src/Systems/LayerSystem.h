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

private:
	void MakeFloor();
	void CheckForDrop();
	bool CheckLine(int i);
	void CheckLines();
	
public:
	bool mLayers[16][12][12]; // height, z, x
	bool mFloor[16][12][12]; // for blocks that are fixed

	BlockSystem* mBlockSystem;
	Tetromino* mActiveTetromino;

	float mMoveInterval;
	float mTimePast;
	float mInterval;

	bool mDropping;
	float mDropInterval;

	bool mGameOver;
	
	glm::ivec3 mActiveTetrominoPosition;
	int mActiveTetrominoBase;
};