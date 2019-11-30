#pragma once
#include "Hollow/Systems/System.h"

struct Tetromino
{
	bool mData[3][3][3]; //Height, z, x
};

class BlockSystem : public Hollow::System
{
	REGISTERSYSTEM(BlockSystem, 2);
public:
	~BlockSystem() {}
	void Init() override;
	void Update() override;
	void AddGameObject(Hollow::GameObject* object) override;

public:
	bool mSpawnBlock;

private:
	Tetromino mShapes[2]; //Type
};