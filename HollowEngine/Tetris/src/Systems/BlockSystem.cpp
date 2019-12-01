#include <Hollow.h>
#include "BlockSystem.h"
#include "Hollow/Common.h"

#include "LayerSystem.h"
#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Managers/InputManager.h"

BlockSystem BlockSystem::instance;

void BlockSystem::Init()
{
	mLayerSystem = Hollow::SystemManager::Instance().GetSystem<LayerSystem>();
	
	// Init tetrominos
	Tetromino3* L = new Tetromino3();

	L->mData[2][1][1] = L->mData[1][1][1] = L->mData[0][1][1] = L->mData[0][1][2] = true;
	mTetrominos[0] = L;

	Tetromino3* S = new Tetromino3();
	S->mData[2][1][0] = S->mData[1][1][0] = S->mData[1][1][1] = S->mData[0][1][1] = true;
	mTetrominos[1] = S;

	Tetromino3* T = new Tetromino3();
	T->mData[1][1][2] = T->mData[1][1][0] = T->mData[1][1][1] = T->mData[0][1][1] = true;
	mTetrominos[2] = T;

	// I
	Tetromino4* I = new Tetromino4();
	I->mData[0][2][1] = I->mData[1][2][1] = I->mData[2][2][1] = I->mData[3][2][1] = true;
	mTetrominos[3] = I;

	Tetromino2* O = new Tetromino2();
	mTetrominos[4] = O;
	
	mSpawnBlock = true;
}

void BlockSystem::CopyTetromino3(bool src[3][3][3], bool dest[][10][10], glm::ivec3 position)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				dest[i + position.x][j + position.y][k + position.z] = src[i][j][k];
			}
		}
	}
	
}

void BlockSystem::CopyTetromino4(bool src[4][4][4], bool dest[][10][10], glm::ivec3 position)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				dest[i + position.x][j + position.y][k + position.z] = src[i][j][k];
			}
		}
	}
}

void BlockSystem::CopyTetromino2(bool src[2][2][2], bool dest[][10][10], glm::ivec3 position)
{
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				dest[i + position.x][j + position.y][k + position.z] = src[i][j][k];
			}
		}
	}
}

void BlockSystem::Rotate(Tetromino& data, Rotation rotation)
{
	switch(rotation)
	{
	case X:
		switch(data.size)
		{
		case 2:
			break;
		case 3:
			RotateAboutX(static_cast<Tetromino3&>(data));
			break;
		case 4:
			RotateAboutX(static_cast<Tetromino4&>(data));
			break;
		}
		break;

	case Y:
		switch (data.size)
		{
		case 2:
			break;
		case 3:
			RotateAboutY(static_cast<Tetromino3&>(data));
			break;
		case 4:
			RotateAboutY(static_cast<Tetromino4&>(data));
			break;
		}
		break;

	case Z:
		switch (data.size)
		{
		case 2:
			break;
		case 3:
			RotateAboutZ(static_cast<Tetromino3&>(data));
			break;
		case 4:
			RotateAboutZ(static_cast<Tetromino4&>(data));
			break;
		}
		break;
	}
}

void BlockSystem::CopyTetromino(Tetromino& t, bool dest[][10][10], glm::ivec3 position)
{
	switch(t.size)
	{
	case 2:
	{
		Tetromino2& t2 = static_cast<Tetromino2&>(t);
		CopyTetromino2(t2.mData, dest, position);
	}
		break;
	case 3:
	{
		Tetromino3& t3 = static_cast<Tetromino3&>(t);
		CopyTetromino3(t3.mData, dest, position);
	}
		break;
	case 4:
	{
		Tetromino4& t4 = static_cast<Tetromino4&>(t);
		CopyTetromino4(t4.mData, dest, position);
	}
		break;
	}
}

void BlockSystem::Update()
{
	if(mSpawnBlock)
	{
		delete mLayerSystem->mActiveTetromino;
		mLayerSystem->mActiveTetromino = mTetrominos[0]->Copy();
		mLayerSystem->mActiveTetrominoPosition = glm::ivec3(12, 4, 4);
		mSpawnBlock = false;
	}
	if(Hollow::InputManager::Instance().IsKeyTriggered(SDL_SCANCODE_K))
	{
		Rotate(*mLayerSystem->mActiveTetromino, Z);
	}
	if (Hollow::InputManager::Instance().IsKeyTriggered(SDL_SCANCODE_J))
	{
		Rotate(*mLayerSystem->mActiveTetromino, X);
	}
	if (Hollow::InputManager::Instance().IsKeyTriggered(SDL_SCANCODE_L))
	{
		Rotate(*mLayerSystem->mActiveTetromino, Y);
	}
}

void BlockSystem::AddGameObject(Hollow::GameObject* object)
{
}

void BlockSystem::RotateAboutZ(Tetromino3& data)
{
	bool workingArray[3][3] = { false };

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			bool res = false;
			for(int k = 0; k < 3; ++k)
			{
				res = res || data.mData[2 - i][k][j];
			}
			workingArray[i][j] = res;
		}
	}

	// Mirror working array
	for(int i = 0; i < 3; ++i)
	{
		std::swap(workingArray[0][i], workingArray[2][i]);
		for(int j = 0; j < 3; ++j)
		{
			std::swap(data.mData[2][j][i], data.mData[0][j][i]);
		}
	}

	// Transpose working array
	for(int i = 0; i < 3; ++i)
	{
		for(int j = i; j < 3; ++j)
		{
			if(i==j)
			{
				continue;
			}
			std::swap(workingArray[i][j], workingArray[j][i]);
			for(int k = 0; k < 3; ++k)
			{
				std::swap(data.mData[2 - i][k][j], data.mData[2-j][k][i]);
			}
		}
	}
}

void BlockSystem::RotateAboutZ(Tetromino4& data)
{
	bool workingArray[4][4] = { false };

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			bool res = false;
			for (int k = 0; k < 4; ++k)
			{
				res = res || data.mData[3 - i][k][j];
			}
			workingArray[i][j] = res;
		}
	}

	// Mirror working array
	for (int i = 0; i < 4; ++i)
	{
		std::swap(workingArray[0][i], workingArray[3][i]);
		std::swap(workingArray[1][i], workingArray[2][i]);
		for (int j = 0; j < 4; ++j)
		{
			std::swap(data.mData[3][j][i], data.mData[0][j][i]);
			std::swap(data.mData[2][j][i], data.mData[1][j][i]);
		}
	}

	// Transpose working array
	for (int i = 0; i < 4; ++i)
	{
		for (int j = i; j < 4; ++j)
		{
			if (i == j)
			{
				continue;
			}
			std::swap(workingArray[i][j], workingArray[j][i]);
			for (int k = 0; k < 4; ++k)
			{
				std::swap(data.mData[3 - i][k][j], data.mData[3 - j][k][i]);
			}
		}
	}
}

void BlockSystem::RotateAboutX(Tetromino3& data)
{
	bool workingArray[3][3] = { false };

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			bool res = false;
			for (int k = 0; k < 3; ++k)
			{
				res = res || data.mData[2 - i][j][k];
			}
			workingArray[i][j] = res;
		}
	}

	// Mirror working array
	for (int i = 0; i < 3; ++i)
	{
		std::swap(workingArray[0][i], workingArray[2][i]);
		for (int j = 0; j < 3; ++j)
		{
			std::swap(data.mData[2][i][j], data.mData[0][i][j]);
		}
	}

	// Transpose working array
	for (int i = 0; i < 3; ++i)
	{
		for (int j = i; j < 3; ++j)
		{
			if (i == j)
			{
				continue;
			}
			std::swap(workingArray[i][j], workingArray[j][i]);
			for (int k = 0; k < 3; ++k)
			{
				std::swap(data.mData[2 - i][j][k], data.mData[2 - j][i][k]);
			}
		}
	}
}

void BlockSystem::RotateAboutX(Tetromino4& data)
{
	bool workingArray[4][4] = { false };

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			bool res = false;
			for (int k = 0; k < 4; ++k)
			{
				res = res || data.mData[3 - i][j][k];
			}
			workingArray[i][j] = res;
		}
	}

	// Mirror working array
	for (int i = 0; i < 4; ++i)
	{
		std::swap(workingArray[0][i], workingArray[3][i]);
		std::swap(workingArray[1][i], workingArray[2][i]);
		for (int j = 0; j < 4; ++j)
		{
			std::swap(data.mData[3][i][j], data.mData[0][i][j]);
			std::swap(data.mData[2][i][j], data.mData[1][i][j]);
		}
	}

	// Transpose working array
	for (int i = 0; i < 4; ++i)
	{
		for (int j = i; j < 4; ++j)
		{
			if (i == j)
			{
				continue;
			}
			std::swap(workingArray[i][j], workingArray[j][i]);
			for (int k = 0; k < 4; ++k)
			{
				std::swap(data.mData[3 - i][j][k], data.mData[3 - j][i][k]);
			}
		}
	}
}

void BlockSystem::RotateAboutY(Tetromino3& data)
{
	bool workingArray[3][3] = { false };

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			bool res = false;
			for (int k = 0; k < 3; ++k)
			{
				res = res || data.mData[k][i][j];
			}
			workingArray[i][j] = res;
		}
	}

	// Mirror working array
	for (int i = 0; i < 3; ++i)
	{
		std::swap(workingArray[0][i], workingArray[2][i]);
		for (int j = 0; j < 3; ++j)
		{
			std::swap(data.mData[j][i][2], data.mData[j][i][0]);
		}
	}

	// Transpose working array
	for (int i = 0; i < 3; ++i)
	{
		for (int j = i; j < 3; ++j)
		{
			if (i == j)
			{
				continue;
			}
			std::swap(workingArray[i][j], workingArray[j][i]);
			for (int k = 0; k < 3; ++k)
			{
				std::swap(data.mData[k][j][2-i], data.mData[k][i][2-j]);
			}
		}
	}
}

void BlockSystem::RotateAboutY(Tetromino4& data)
{
	bool workingArray[4][4] = { false };

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			bool res = false;
			for (int k = 0; k < 4; ++k)
			{
				res = res || data.mData[k][i][j];
			}
			workingArray[i][j] = res;
		}
	}

	// Mirror working array
	for (int i = 0; i < 4; ++i)
	{
		std::swap(workingArray[0][i], workingArray[3][i]);
		std::swap(workingArray[1][i], workingArray[2][i]);
		for (int j = 0; j < 4; ++j)
		{
			std::swap(data.mData[j][i][3], data.mData[j][i][0]);
			std::swap(data.mData[j][i][2], data.mData[j][i][1]);
		}
	}

	// Transpose working array
	for (int i = 0; i < 4; ++i)
	{
		for (int j = i; j < 4; ++j)
		{
			if (i == j)
			{
				continue;
			}
			std::swap(workingArray[i][j], workingArray[j][i]);
			for (int k = 0; k < 4; ++k)
			{
				std::swap(data.mData[k][j][3 - i], data.mData[k][i][3 - j]);
			}
		}
	}
}