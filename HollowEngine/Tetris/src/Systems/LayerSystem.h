#pragma once
#include "Hollow/Systems/System.h"
class BlockSystem;
struct Tetromino;

/// <summary>
/// Class Layer System.
/// Implements the "Hollow::System"
/// </summary>
/// <seealso cref="Hollow::System" />
class LayerSystem : public Hollow::System
{
	REGISTERSYSTEM(LayerSystem, 2);
public:
	
	/// <summary>
	/// Initializes system data.
	/// </summary>
	void Init() override;
	
	/// <summary>
	/// Updates system data.
	/// </summary>
	void Update() override;
	
	/// <summary>
	/// Adds the game object.
	/// </summary>
	/// <param name="object">The game object.</param>
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