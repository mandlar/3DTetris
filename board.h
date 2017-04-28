#include <block.h>
class Board
{
public:
	Board();
	Block gameBoard[4][4][24];
	int height;
	int width;
	int depth;
	void clearBoard();
	
private:
	
};

extern Board TetrisBoard;