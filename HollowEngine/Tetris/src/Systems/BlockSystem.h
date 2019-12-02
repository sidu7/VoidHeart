#pragma once
#include "Hollow/Systems/System.h"

struct Tetromino
{
	int size;
	virtual Tetromino* Copy() = 0;
	Tetromino(int size) : size(size)
	{		
	}
};

enum Rotation
{
	X,
	Y,
	Z
};

enum Direction
{
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

struct Tetromino3 : Tetromino
{
	bool mData[3][3][3]; //Height, z, x

	Tetromino* Copy() override
	{
		Tetromino3* t3 = new Tetromino3();
		memcpy(t3->mData, mData, size * size * size * sizeof(bool));
		return t3;
	}
	
	Tetromino3() : Tetromino(3)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					mData[i][j][k] = false;
				}
			}
		}
	}
};

struct Tetromino4 : Tetromino
{
	bool mData[4][4][4];
	Tetromino* Copy() override
	{
		Tetromino4* t4 = new Tetromino4();
		memcpy(t4->mData, mData, size * size * size * sizeof(bool));
		return t4;
	}
	Tetromino4() : Tetromino(4)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				for (int k = 0; k < 4; ++k)
				{
					mData[i][j][k] = false;
				}
			}
		}
	}
};

struct Tetromino2 : Tetromino
{
	bool mData[2][2][2];
	Tetromino* Copy() override
	{
		Tetromino2* t2 = new Tetromino2();
		memcpy(t2->mData, mData, size * size * size * sizeof(bool));
		return t2;
	}
	
	Tetromino2() : Tetromino(2)
	{
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				for (int k = 0; k < 2; ++k)
				{
					mData[i][j][k] = true;
				}
			}
		}
	}
};

class LayerSystem;

class BlockSystem : public Hollow::System
{
	REGISTERSYSTEM(BlockSystem, 2);
public:
	~BlockSystem() {}
	void Init() override;
	void Updato();
	void Update() {}
	void AddGameObject(Hollow::GameObject* object) override;


	void CopyTetromino(Tetromino& t, bool dest[][12][12], glm::ivec3 position);
	
private:
	void CopyTetromino3(bool src[3][3][3], bool dest[][12][12], glm::ivec3 position);
	void CopyTetromino4(bool src[4][4][4], bool dest[][12][12], glm::ivec3 position);
	void CopyTetromino2(bool src[2][2][2], bool dest[][12][12], glm::ivec3 position);
	void RotateAboutZ(Tetromino3& data);
	void RotateAboutZ(Tetromino4& data);
	void RotateAboutY(Tetromino3& data);
	void RotateAboutY(Tetromino4& data);
	void RotateAboutX(Tetromino3& data);
	void RotateAboutX(Tetromino4& data);

	bool CheckForBound(Direction dir);

	void Rotate(Tetromino& data, Rotation rotation);
	
public:
	bool mSpawnBlock;

private:
	Tetromino3 mShapes[3]; // L, S, and T
	Tetromino4 mBar;
	Tetromino2 mCube;
	
	Tetromino* mTetrominos[5]; // L,S,T,I,O

	LayerSystem* mLayerSystem;

	bool mSpawnBox;
};