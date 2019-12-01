#pragma once
#include "Hollow/Systems/System.h"

struct Tetromino3
{
	bool mData[3][3][3]; //Height, z, x
	Tetromino3()
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

struct Tetromino4
{
	bool mData[4][4][4];
	Tetromino4()
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

struct Tetromino2
{
	bool mData[2][2][2];
	Tetromino2()
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

class BlockSystem : public Hollow::System
{
	REGISTERSYSTEM(BlockSystem, 2);
public:
	~BlockSystem() {}
	void Init() override;
	void Update() override;
	void AddGameObject(Hollow::GameObject* object) override;

private:
	Tetromino3 RotateAboutZ(Tetromino3& data);
	Tetromino4 RotateAboutZ(Tetromino4& data);
	Tetromino3 RotateAboutY(Tetromino3& data);
	Tetromino3 RotateAboutX(Tetromino3& data);
	void CopyTetromino3(bool src[3][3][3], bool dest[][10][10]);
	void CopyTetromino4(bool src[4][4][4], bool dest[][10][10]);
	void CopyTetromino2(bool src[2][2][2], bool dest[][10][10]);

public:
	bool mSpawnBlock;

private:
	Tetromino3 mShapes[3]; // L, S, and T
	Tetromino4 mBar;
	Tetromino2 mCube;
};