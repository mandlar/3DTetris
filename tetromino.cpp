#include <tetromino.h>
#include <board.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>





enum { ISHAPE, LSHAPE, JSHAPE, SSHAPE, ZSHAPE, TSHAPE, OSHAPE};



// create a new random Tetromino piece
Tetromino::Tetromino()
{
	Sleep(50); // to ensure that no two beginning tetrominos are the same
	srand(GetTickCount());

	// choose a random shape
	int shape = (int)((double)rand() / ((double)RAND_MAX + 1) * 7);
	
	//int shape = rand() % 7;
	// TODO: future considerations if the gameboard size can change.  Find the "center" of the board and make block[1] be placed there, and adjust the other blocks accordingly
	if(shape == ISHAPE)
	{
		type = ISHAPE;
		// CYAN
		float red = 0;
		float green = 1;
		float blue = 1;
		this->blocks[0].x = 0;
		this->blocks[0].y = 2;
		this->blocks[0].z = 19;
		this->blocks[0].red = red;
		this->blocks[0].green = green;
		this->blocks[0].blue = blue;

		this->blocks[1].x = 1;
		this->blocks[1].y = 2;
		this->blocks[1].z = 19;
		this->blocks[1].red = red;
		this->blocks[1].green = green;
		this->blocks[1].blue = blue;

		this->blocks[2].x = 2;
		this->blocks[2].y = 2;
		this->blocks[2].z = 19;
		this->blocks[2].red = red;
		this->blocks[2].green = green;
		this->blocks[2].blue = blue;

		this->blocks[3].x = 3;
		this->blocks[3].y = 2;
		this->blocks[3].z = 19;
		this->blocks[3].red = red;
		this->blocks[3].green = green;
		this->blocks[3].blue = blue;
	}
	else if(shape == LSHAPE)
	{
		type = LSHAPE;
		//ORANGE
		float red = 1;
		float green = 0.65;
		float blue = 0;
		this->blocks[0].x = 2;
		this->blocks[0].y = 0;
		this->blocks[0].z = 19;
		this->blocks[0].red = red;
		this->blocks[0].green = green;
		this->blocks[0].blue = blue;

		this->blocks[1].x = 2;
		this->blocks[1].y = 1;
		this->blocks[1].z = 19;
		this->blocks[1].red = red;
		this->blocks[1].green = green;
		this->blocks[1].blue = blue;

		this->blocks[2].x = 2;
		this->blocks[2].y = 2;
		this->blocks[2].z = 19;
		this->blocks[2].red = red;
		this->blocks[2].green = green;
		this->blocks[2].blue = blue;

		this->blocks[3].x = 1;
		this->blocks[3].y = 2;
		this->blocks[3].z = 19;
		this->blocks[3].red = red;
		this->blocks[3].green = green;
		this->blocks[3].blue = blue;
	}
	else if(shape == JSHAPE)
	{
		type = JSHAPE;
		//blue
		float red = 0;
		float green = 0;
		float blue = 1;
		this->blocks[0].x = 1;
		this->blocks[0].y = 0;
		this->blocks[0].z = 19;
		this->blocks[0].red = red;
		this->blocks[0].green = green;
		this->blocks[0].blue = blue;

		this->blocks[1].x = 1;
		this->blocks[1].y = 1;
		this->blocks[1].z = 19;
		this->blocks[1].red = red;
		this->blocks[1].green = green;
		this->blocks[1].blue = blue;

		this->blocks[2].x = 1;
		this->blocks[2].y = 2;
		this->blocks[2].z = 19;
		this->blocks[2].red = red;
		this->blocks[2].green = green;
		this->blocks[2].blue = blue;

		this->blocks[3].x = 2;
		this->blocks[3].y = 2;
		this->blocks[3].z = 19;
		this->blocks[3].red = red;
		this->blocks[3].green = green;
		this->blocks[3].blue = blue;
	}
	else if(shape == SSHAPE)
	{
		type = SSHAPE;
		//green
		float red = 0;
		float green = 1;
		float blue = 0;
		this->blocks[0].x = 2;
		this->blocks[0].y = 1;
		this->blocks[0].z = 19;
		this->blocks[0].red = red;
		this->blocks[0].green = green;
		this->blocks[0].blue = blue;

		this->blocks[1].x = 1;
		this->blocks[1].y = 1;
		this->blocks[1].z = 19;
		this->blocks[1].red = red;
		this->blocks[1].green = green;
		this->blocks[1].blue = blue;

		this->blocks[2].x = 1;
		this->blocks[2].y = 2;
		this->blocks[2].z = 19;
		this->blocks[2].red = red;
		this->blocks[2].green = green;
		this->blocks[2].blue = blue;

		this->blocks[3].x = 0;
		this->blocks[3].y = 2;
		this->blocks[3].z = 19;
		this->blocks[3].red = red;
		this->blocks[3].green = green;
		this->blocks[3].blue = blue;
	}
	else if(shape == ZSHAPE)
	{
		type = ZSHAPE;
		//red
		float red = 1;
		float green = 0;
		float blue = 0;
		this->blocks[0].x = 1;
		this->blocks[0].y = 1;
		this->blocks[0].z = 19;
		this->blocks[0].red = red;
		this->blocks[0].green = green;
		this->blocks[0].blue = blue;

		this->blocks[1].x = 2;
		this->blocks[1].y = 1;
		this->blocks[1].z = 19;
		this->blocks[1].red = red;
		this->blocks[1].green = green;
		this->blocks[1].blue = blue;

		this->blocks[2].x = 2;
		this->blocks[2].y = 2;
		this->blocks[2].z = 19;
		this->blocks[2].red = red;
		this->blocks[2].green = green;
		this->blocks[2].blue = blue;

		this->blocks[3].x = 3;
		this->blocks[3].y = 2;
		this->blocks[3].z = 19;
		this->blocks[3].red = red;
		this->blocks[3].green = green;
		this->blocks[3].blue = blue;
	}
	else if(shape == TSHAPE)
	{
		type = TSHAPE;
		//purple
		float red = 0.63;
		float green = 0.13;
		float blue = 0.94;
		this->blocks[0].x = 1;
		this->blocks[0].y = 0;
		this->blocks[0].z = 19;
		this->blocks[0].red = red;
		this->blocks[0].green = green;
		this->blocks[0].blue = blue;

		this->blocks[1].x = 1;
		this->blocks[1].y = 1;
		this->blocks[1].z = 19;
		this->blocks[1].red = red;
		this->blocks[1].green = green;
		this->blocks[1].blue = blue;

		this->blocks[2].x = 1;
		this->blocks[2].y = 2;
		this->blocks[2].z = 19;
		this->blocks[2].red = red;
		this->blocks[2].green = green;
		this->blocks[2].blue = blue;

		this->blocks[3].x = 2;
		this->blocks[3].y = 1;
		this->blocks[3].z = 19;
		this->blocks[3].red = red;
		this->blocks[3].green = green;
		this->blocks[3].blue = blue;
	}
	else if(shape == OSHAPE)
	{
		type = OSHAPE;
		float red = 1;
		float green = 1;
		float blue = 0;
		this->blocks[0].x = 1;
		this->blocks[0].y = 1;
		this->blocks[0].z = 19;
		this->blocks[0].red = red;
		this->blocks[0].green = green;
		this->blocks[0].blue = blue;

		this->blocks[1].x = 2;
		this->blocks[1].y = 1;
		this->blocks[1].z = 19;
		this->blocks[1].red = red;
		this->blocks[1].green = green;
		this->blocks[1].blue = blue;

		this->blocks[2].x = 2;
		this->blocks[2].y = 2;
		this->blocks[2].z = 19;
		this->blocks[2].red = red;
		this->blocks[2].green = green;
		this->blocks[2].blue = blue;

		this->blocks[3].x = 1;
		this->blocks[3].y = 2;
		this->blocks[3].z = 19;
		this->blocks[3].red = red;
		this->blocks[3].green = green;
		this->blocks[3].blue = blue;
	}
}

// check if the block is directly above the bottom of the board or a part of the board
bool Tetromino::DetectBoardCollision()
{
	for(int i = 0; i < 4; i++)
	{
		//check if it is one above the bottom of the board
		if(blocks[i].z == 0)
			return true;
		//check if it is one above a block attached to the board
		if(TetrisBoard.gameBoard[blocks[i].x][blocks[i].y][blocks[i].z-1].exists == true)
			return true;
	}
	return false;
}

// checks if rotation either: 1) goes outside of the board or 2) rotation ends on a block attached to the board
// returns true if either collision happens
// returns false if neither collision happens
bool Tetromino::RotateCollision(Block pieces[])
{
	for(int i = 0; i < 4; i++)
	{
		if(pieces[i].x < 0 | pieces[i].x > TetrisBoard.width-1 | pieces[i].y < 0 | pieces[i].y > TetrisBoard.height-1 | (TetrisBoard.gameBoard[pieces[i].x][pieces[i].y][pieces[i].z].exists == true))
		{
			return true;
		}
	}
	return false;
}

void Tetromino::RotateXCW()
{
	// copy all of the blocks
	Block tempBlocks[4];
	for(int i = 0; i < 4; i++)
	{
		tempBlocks[i] = blocks[i];
		// block[1] is center, translate it to 0,0, and move all other blocks too.
		tempBlocks[i].y = tempBlocks[i].y - blocks[1].y;
		tempBlocks[i].z = tempBlocks[i].z - blocks[1].z;
	}
	// now rotate by the new origin 0,0
	// CW = (y,z) = (z, -y)
	for(int i = 0; i < 4; i++)
	{
		int newy = tempBlocks[i].z;
		int newz = -tempBlocks[i].y;
		tempBlocks[i].y = newy;
		tempBlocks[i].z = newz;
	}
	// now translate all the blocks back from the origin
	for(int i = 0; i < 4; i++)
	{
		tempBlocks[i].y = tempBlocks[i].y + blocks[1].y;
		tempBlocks[i].z = tempBlocks[i].z + blocks[1].z;
	}

	if(RotateCollision(tempBlocks))
	{
		return;
	}

	// copy the tempBlocks to blocks
	for(int i = 0; i < 4; i++)
	{
		blocks[i] = tempBlocks[i];
	}
}

void Tetromino::RotateXCCW()
{
	// copy all of the blocks
	Block tempBlocks[4];
	for(int i = 0; i < 4; i++)
	{
		tempBlocks[i] = blocks[i];
		// block[1] is center, translate it to 0,0, and move all other blocks too.
		tempBlocks[i].y = tempBlocks[i].y - blocks[1].y;
		tempBlocks[i].z = tempBlocks[i].z - blocks[1].z;
	}
	// now rotate by the new origin 0,0
	// CCW = (y,z) = (-z, y)
	for(int i = 0; i < 4; i++)
	{
		int newy = -tempBlocks[i].z;
		int newz = tempBlocks[i].y;
		tempBlocks[i].y = newy;
		tempBlocks[i].z = newz;
	}
	// now translate all the blocks back from the origin
	for(int i = 0; i < 4; i++)
	{
		tempBlocks[i].y = tempBlocks[i].y + blocks[1].y;
		tempBlocks[i].z = tempBlocks[i].z + blocks[1].z;
	}

	if(RotateCollision(tempBlocks))
	{
		return;
	}
	// copy the tempBlocks to blocks
	for(int i = 0; i < 4; i++)
	{
		blocks[i] = tempBlocks[i];
	}
}

void Tetromino::RotateYCW()
{
	// copy all of the blocks
	Block tempBlocks[4];
	for(int i = 0; i < 4; i++)
	{
		tempBlocks[i] = blocks[i];
		// block[1] is center, translate it to 0,0, and move all other blocks too.
		tempBlocks[i].x = tempBlocks[i].x - blocks[1].x;
		tempBlocks[i].z = tempBlocks[i].z - blocks[1].z;
	}
	// now rotate by the new origin 0,0
	// CW = (x,z) = (z, -x)
	for(int i = 0; i < 4; i++)
	{
		int newx = tempBlocks[i].z;
		int newz = -tempBlocks[i].x;
		tempBlocks[i].x = newx;
		tempBlocks[i].z = newz;
	}
	// now translate all the blocks back from the origin
	for(int i = 0; i < 4; i++)
	{
		tempBlocks[i].x = tempBlocks[i].x + blocks[1].x;
		tempBlocks[i].z = tempBlocks[i].z + blocks[1].z;
	}

	if(RotateCollision(tempBlocks))
	{
		return;
	}
	// copy the tempBlocks to blocks
	for(int i = 0; i < 4; i++)
	{
		blocks[i] = tempBlocks[i];
	}
}

void Tetromino::RotateYCCW()
{
	// copy all of the blocks
	Block tempBlocks[4];
	for(int i = 0; i < 4; i++)
	{
		tempBlocks[i] = blocks[i];
		// block[1] is center, translate it to 0,0, and move all other blocks too.
		tempBlocks[i].x = tempBlocks[i].x - blocks[1].x;
		tempBlocks[i].z = tempBlocks[i].z - blocks[1].z;
	}
	// now rotate by the new origin 0,0
	// CCW = (x,z) = (-z, x)
	for(int i = 0; i < 4; i++)
	{
		int newx = -tempBlocks[i].z;
		int newz = tempBlocks[i].x;
		tempBlocks[i].x = newx;
		tempBlocks[i].z = newz;
	}
	// now translate all the blocks back from the origin
	for(int i = 0; i < 4; i++)
	{
		tempBlocks[i].x = tempBlocks[i].x + blocks[1].x;
		tempBlocks[i].z = tempBlocks[i].z + blocks[1].z;
	}

	if(RotateCollision(tempBlocks))
	{
		return;
	}
	// copy the tempBlocks to blocks
	for(int i = 0; i < 4; i++)
	{
		blocks[i] = tempBlocks[i];
	}
}

void Tetromino::RotateZCW()
{
	// copy all of the blocks
	Block tempBlocks[4];
	for(int i = 0; i < 4; i++)
	{
		tempBlocks[i] = blocks[i];
		// block[1] is center, translate it to 0,0, and move all other blocks too.
		tempBlocks[i].x = tempBlocks[i].x - blocks[1].x;
		tempBlocks[i].y = tempBlocks[i].y - blocks[1].y;
	}
	// now rotate by the new origin 0,0
	// CW = (x,y) = (y, -x)
	for(int i = 0; i < 4; i++)
	{
		int newx = tempBlocks[i].y;
		int newy = -tempBlocks[i].x;
		tempBlocks[i].x = newx;
		tempBlocks[i].y = newy;
	}
	// now translate all the blocks back from the origin
	for(int i = 0; i < 4; i++)
	{
		tempBlocks[i].x = tempBlocks[i].x + blocks[1].x;
		tempBlocks[i].y = tempBlocks[i].y + blocks[1].y;
	}

	if(RotateCollision(tempBlocks))
	{
		return;
	}
	// copy the tempBlocks to blocks
	for(int i = 0; i < 4; i++)
	{
		blocks[i] = tempBlocks[i];
	}
}

void Tetromino::RotateZCCW()
{
	// copy all of the blocks
	Block tempBlocks[4];
	for(int i = 0; i < 4; i++)
	{
		tempBlocks[i] = blocks[i];
		// block[1] is center, translate it to 0,0, and move all other blocks too.
		tempBlocks[i].x = tempBlocks[i].x - blocks[1].x;
		tempBlocks[i].y = tempBlocks[i].y - blocks[1].y;
	}
	// now rotate by the new origin 0,0
	// CCW = (x,y) = (-y, x)
	for(int i = 0; i < 4; i++)
	{
		int newx = -tempBlocks[i].y;
		int newy = tempBlocks[i].x;
		tempBlocks[i].x = newx;
		tempBlocks[i].y = newy;
	}
	// now translate all the blocks back from the origin
	for(int i = 0; i < 4; i++)
	{
		tempBlocks[i].x = tempBlocks[i].x + blocks[1].x;
		tempBlocks[i].y = tempBlocks[i].y + blocks[1].y;
	}

	if(RotateCollision(tempBlocks))
	{
		return;
	}
	// copy the tempBlocks to blocks
	for(int i = 0; i < 4; i++)
	{
		blocks[i] = tempBlocks[i];
	}
}


// "up" key
void Tetromino::MoveBack()
{
	bool collision = false;
	
	for(int i = 0; i < 4; i++)
	{
		// TODO: this could be consolidated into a function?
		// Check if any block of the tetromino hits a map edge
		if(this->blocks[i].y == 0)
			collision = true;
		// Check if any block hits any block attached to the board
		if(TetrisBoard.gameBoard[this->blocks[i].x][this->blocks[i].y-1][this->blocks[i].z].exists == true)
			collision = true;
	}

	// Move each block
	if(collision == false)
	{
		for(int i = 0; i < 4; i++)
		{
			this->blocks[i].y = this->blocks[i].y -1;
		}
	}

}

// "down" key
void Tetromino::MoveForward()
{
	bool collision = false;
	// Check if any block of the tetromino hits a map edge
	for(int i = 0; i < 4; i++)
	{
		if(this->blocks[i].y == TetrisBoard.height-1)
			collision = true;
		// Check if any block hits any block attached to the board
		if(TetrisBoard.gameBoard[this->blocks[i].x][this->blocks[i].y+1][this->blocks[i].z].exists == true)
			collision = true;
	}

	// Move each block
	if(collision == false)
	{
		for(int i = 0; i < 4; i++)
		{
			this->blocks[i].y = this->blocks[i].y +1;
		}
	}
}

void Tetromino::MoveRight()
{
	bool collision = false;
	// Check if any block of the tetromino hits a map edge
	for(int i = 0; i < 4; i++)
	{
		if(this->blocks[i].x == TetrisBoard.width-1)
			collision = true;
		// Check if any block hits any block attached to the board
		if(TetrisBoard.gameBoard[this->blocks[i].x+1][this->blocks[i].y][this->blocks[i].z].exists == true)
			collision = true;
	}

	// Move each block
	if(collision == false)
	{
		for(int i = 0; i < 4; i++)
		{
			this->blocks[i].x = this->blocks[i].x +1;
		}
	}
}

void Tetromino::MoveLeft()
{
	bool collision = false;
	// Check if any block of the tetromino hits a map edge
	for(int i = 0; i < 4; i++)
	{
		if(this->blocks[i].x == 0)
			collision = true;
		// Check if any block hits any block attached to the board
		if(TetrisBoard.gameBoard[this->blocks[i].x-1][this->blocks[i].y][this->blocks[i].z].exists == true)
			collision = true;

	}

	// Move each block
	if(collision == false)
	{
		for(int i = 0; i < 4; i++)
		{
			this->blocks[i].x = this->blocks[i].x -1;
		}
	}
}


// returns true if the piece became attached to the board
// returns false if it doesn't
bool Tetromino::DropOne()
{
	// check if the piece will attach itself to the board (bottom level or any other piece)
	if(DetectBoardCollision())
	{
		// attach the piece to the board
		for(int i = 0; i < 4; i++)
		{
			TetrisBoard.gameBoard[blocks[i].x][blocks[i].y][blocks[i].z].exists = true;
			TetrisBoard.gameBoard[blocks[i].x][blocks[i].y][blocks[i].z].red = blocks[i].red;
			TetrisBoard.gameBoard[blocks[i].x][blocks[i].y][blocks[i].z].blue = blocks[i].blue;
			TetrisBoard.gameBoard[blocks[i].x][blocks[i].y][blocks[i].z].green = blocks[i].green;
		}
		return true;
	}
	else
	{
	// else drop the piece down by one block.
		for(int i = 0; i < 4; i++)
		{
			this->blocks[i].z = this->blocks[i].z -1;
		}
		return false;
	}
}

// same as above but DOES NOT attach piece to board when it reaches the bottom
// used for shadow pieces
// true if hits bottom
// false otherwise
bool Tetromino::ShadowDropOne()
{
	// check if the piece will attach itself to the board (bottom level or any other piece)
	if(DetectBoardCollision())
	{
		// DON'T attach the piece to the board
		return true;
	}
	else
	{
	// else drop the piece down by one block.
		for(int i = 0; i < 4; i++)
		{
			this->blocks[i].z = this->blocks[i].z -1;
		}
		return false;
	}
}

