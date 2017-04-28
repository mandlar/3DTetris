#include <board.h>

void Board::clearBoard()
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int z = 0; z < depth; z++)
			{
				gameBoard[x][y][z].exists = false;
			}
		}
	}
}
Board::Board()
{
	height = 4;
	width = 4;
	depth = 24;

	// clear the board
	this->clearBoard();
}