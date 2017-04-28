#include <block.h>

class Tetromino
{
public:
	Tetromino();
	Block blocks[4];
	int type;
	bool RotateCollision(Block pieces[]);
	void RotateXCW();
	void RotateXCCW();
	void RotateYCW();
	void RotateYCCW();
	void RotateZCW();
	void RotateZCCW();
	void MoveForward();
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	bool DropOne();
	bool DetectBoardCollision();
	bool ShadowDropOne();
private:
	
};