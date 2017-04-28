#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <cstdio>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <string>
#include <time.h>
#include <tetromino.h>
#include <board.h>
#include <texture.h>
#include <utility>
using namespace std;


bool LoadTGA(Texture *, char *); // For TGA Textures

// game variables
Board TetrisBoard;
Tetromino currentTetromino;
int CameraAngle = 0;
int CameraPitch = 0;
int CameraDistance = -30;
int BoardDistance = -20;
int level = 1;
int lines = 0;
int score = 0;
int TimeToDrop = 5000; // in ms, the time required for a piece to drop by one
Texture texture[7];	//Storage for textures
Tetromino nextTetromino;
Tetromino shadowTetromino;
bool gameover = false;
enum { ISHAPE, LSHAPE, JSHAPE, SSHAPE, ZSHAPE, TSHAPE, OSHAPE};

// Used to render font to screen
void renderBitmapString(float x, float y, float z, void *font, char *string) 
{  
  char *c;
  glRasterPos3f(x, y,z);
  for (c=string; *c != '\0'; c++) 
  {
    glutBitmapCharacter(font, *c);
  }
}



//NOTE: some code here borrowed from TGALoader
bool LoadGLTextures()											// Load Bitmaps And Convert To Textures
{
	// select modulate to mix texture with color for shading
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	bool Status = false;											// Status Indicator

	// Load The Bitmap, Check For Errors.
	if (LoadTGA(&texture[0], "block.tga") && LoadTGA(&texture[1], "logo.tga"))// && LoadTGA(&texture[2], "leftside_02.tga") && LoadTGA(&texture[3], "leftside_03.tga") && LoadTGA(&texture[4], "rightside_01.tga") && LoadTGA(&texture[5], "rightside_02.tga") && LoadTGA(&texture[6], "rightside_03.tga"))
	{
		Status = true;											// Set The Status To TRUE

		for (int loop=0; loop<7; loop++)						// Loop Through Both Textures
		{
			// Typical Texture Generation Using Data From The TGA 
			glGenTextures(1, &texture[loop].texID);				// Create The Texture
			glBindTexture(GL_TEXTURE_2D, texture[loop].texID);
			glTexImage2D(GL_TEXTURE_2D, 0, texture[loop].bpp / 8, texture[loop].width, texture[loop].height, 0, texture[loop].type, GL_UNSIGNED_BYTE, texture[loop].imageData);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			if (texture[loop].imageData)						// If Texture Image Exists 
			{
				free(texture[loop].imageData);					// Free The Texture Image Memory
			}
		}
	}
	return Status;												
}

// true if game is over
// false o.w.
bool checkGameOver()
{
	if(TetrisBoard.gameBoard[0][2][19].exists == true || TetrisBoard.gameBoard[1][2][19].exists == true || TetrisBoard.gameBoard[2][2][19].exists == true || TetrisBoard.gameBoard[3][2][19].exists == true || TetrisBoard.gameBoard[2][0][19].exists == true || TetrisBoard.gameBoard[2][1][19].exists == true || TetrisBoard.gameBoard[1][0][19].exists == true || TetrisBoard.gameBoard[1][1][19].exists == true)
		return true;
	else
		return false;
}

// post attach clean up
// create a new tetromino
// check if a row is clearable, if so, then clear it.
// update game information (score, level, lines)
void attachedToBoard()
{
	int linesCleared = 0;
	for(int z = 0; z < TetrisBoard.depth; z++)
	{
		bool levelIsClear = true;
		for(int x = 0; x < TetrisBoard.width; x++)
		{
			for(int y = 0; y < TetrisBoard.height; y++)
			{
				if(TetrisBoard.gameBoard[x][y][z].exists != true)
				{
					levelIsClear = false;
				}
			}
		}
		if(levelIsClear == true)
		{
			// move all levels down one, therefore deleting the complete level
			for(int i = z; i < TetrisBoard.depth-1; i++)
			{
				for(int x = 0; x < TetrisBoard.width; x++)
				{
					for(int y = 0; y < TetrisBoard.height; y++)
					{
						TetrisBoard.gameBoard[x][y][i] = TetrisBoard.gameBoard[x][y][i+1];
					}
				}
			}
			//the top most level is a new blank level
			for(int x = 0; x < TetrisBoard.width; x++)
			{
				for(int y = 0; y < TetrisBoard.height; y++)
				{
					TetrisBoard.gameBoard[x][y][TetrisBoard.depth-1].exists = false;
				}
			}
			linesCleared += 1;
			z = -1; // reset our search
		}
	}
	// Update the score
	if(linesCleared == 1)
		score += (100 * level);
	else if(linesCleared == 2)
		score += (300 * level);
	else if(linesCleared == 3)
		score += (500 * level);
	else if(linesCleared == 4)
		score += (800 * level);
	// Update lines and level
	for(int i = 0; i < linesCleared; i++)
	{
		// Update line clear
		lines += 1;
		// Check if it is a new level
		if(lines % 10 == 0)
		{
			level += 1;
			if(TimeToDrop > 600)
				TimeToDrop -= 200;
		}
	}

	currentTetromino = nextTetromino;
	
	// check if a new tetromino can be placed, if not, GAME OVER!
	if(checkGameOver())
	{
		TimeToDrop = 0;
		gameover = true;
	}
	else
	{
		Tetromino newTetromino;
		nextTetromino = newTetromino;
	}
	
}

void draw()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glClearColor( 0, 0, 0, 0 );
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
	// Draw the UI
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	// Sets up screen to draw by pixels
	glOrtho(0, 800, 0, 600, -1,  1000);

	// Draw all of the text areas
	glColor3f(1.0f, 1.0f, 1.0f);
	if(gameover)
		renderBitmapString(250, 25, 0, GLUT_BITMAP_HELVETICA_18, "Game Over!  Press F2 to try again.");
	renderBitmapString(25, 200, 0, GLUT_BITMAP_HELVETICA_18, "W/S - rotate X axis");
	renderBitmapString(25, 175, 0, GLUT_BITMAP_HELVETICA_18, "A/D - rotate Y axis");
	renderBitmapString(25, 150, 0, GLUT_BITMAP_HELVETICA_18, "Z/X - rotate Z axis");
	renderBitmapString(25, 125, 0, GLUT_BITMAP_HELVETICA_18, "Arrows - move piece");
	renderBitmapString(25, 100, 0, GLUT_BITMAP_HELVETICA_18, "Space - hard drop");
	renderBitmapString(25, 75, 0, GLUT_BITMAP_HELVETICA_18, "R/F - move camera up/down");
	renderBitmapString(25, 50, 0, GLUT_BITMAP_HELVETICA_18, "C/V - move camera left/right");
	renderBitmapString(25, 25, 0, GLUT_BITMAP_HELVETICA_18, "F2 - restart game");
	renderBitmapString(55, 570, 0, GLUT_BITMAP_HELVETICA_18, "Next Piece");

	// Convert ints to strings
	string scoreString;
	string levelString;
	string linesString;
	std::ostringstream a;
	a << scoreString << "" << score;
	scoreString = a.str();
	std::ostringstream b;
	b << levelString << "" << level;
	levelString = b.str();
	std::ostringstream c;
	c << linesString << "" << lines;
	linesString = c.str();
	renderBitmapString(650, 375, 0, GLUT_BITMAP_HELVETICA_18, "Score");
	renderBitmapString(650, 350, 0, GLUT_BITMAP_HELVETICA_18, &scoreString[0]);
	renderBitmapString(650, 300, 0, GLUT_BITMAP_HELVETICA_18, "Level");
	renderBitmapString(650, 275, 0, GLUT_BITMAP_HELVETICA_18, &levelString[0]);
	renderBitmapString(650, 225, 0, GLUT_BITMAP_HELVETICA_18, "Lines");
	renderBitmapString(650, 200, 0, GLUT_BITMAP_HELVETICA_18, &linesString[0]);
	
	// Draw logo
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1].texID); // logo texture
	glBegin(GL_QUADS);		
	glColor3f(1.0f,1.0f,1.0f);	
		glNormal3d(0, 0, 1);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 775, 595, 0.2f);	// Top Right Of The Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(647, 595, 0.2f);	// Top Left Of The Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(647, 447, 0.2f);	// Bottom Left Of The Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(775, 447, 0.2f);	// Bottom Right Of The Quad
	glEnd();
	glDisable(GL_TEXTURE_2D);
		
	// Draw the box that will show the "next piece"	
	glBegin(GL_QUADS);		
	glColor3f(1.0f,1.0f,1.0f);	 
		glNormal3d(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 175, 555.0f, 0.0f);	// Top Right Of The Quad 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(25.0f, 555.0f, 0.0f);	// Top Left Of The Quad 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(25.0f, 405.0f, 0.0f);	// Bottom Left Of The Quad 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(175.0f, 405.0f, 0.0f);	// Bottom Right Of The Quad
	glEnd();
	glBegin(GL_QUADS);		
	glColor3f(0.0f,0.0f,0.0f); 
		glNormal3d(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 160.0f, 540.0f, 0.1f);	// Top Right Of The Quad 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(40.0f, 540.0f, 0.1f);	// Top Left Of The Quad 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(40.0f, 420.0f, 0.1f);	// Bottom Left Of The Quad 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(160.0f, 420.0f, 0.1f);	// Bottom Right Of The Quad 
	glEnd();

	// Determine and draw the next piece
	glEnable(GL_TEXTURE_2D);
	if(nextTetromino.type == ISHAPE)
	{
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);
		glBegin(GL_QUADS);		
		glColor3f(0.0f,1.0f,1.0f);	
			glNormal3d(0, 0, 1);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 70.0f, 495.0f, 0.2f);	// Top Right Of The Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(40.0f, 495.0f, 0.2f);	// Top Left Of The Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f(40.0f, 465.0f, 0.2f);	// Bottom Left Of The Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f(70.0f, 465.0f, 0.2f);	// Bottom Right Of The Quad

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 100.0f, 495.0f, 0.2f);	// Top Right Of The Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(70.0f, 495.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(70.0f, 465.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(100.0f, 465.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 130.0f, 495.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 495.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, 465.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(130.0f, 465.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 160.0f, 495.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(130.0f, 495.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(130.0f, 465.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(160.0f, 465.0f, 0.2f);	// Bottom Right Of The Quad 
		glEnd();

	}
	else if (nextTetromino.type == LSHAPE)
	{
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);
		glBegin(GL_QUADS);		
		glColor3f(1.0f,0.65f,0.0f);	
			glNormal3d(0, 0, 1);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 85.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(55.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(55.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(85.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 115.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(85.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(85.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(115.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 145.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(115.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(115.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(145.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 85.0f, 480.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(55.0f, 480.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(55.0f, 450.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(85.0f, 450.0f, 0.2f);	// Bottom Right Of The Quad 
		glEnd();
	}
	else if (nextTetromino.type == JSHAPE)
	{
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);
		glBegin(GL_QUADS);	
		glColor3f(0.0f,0.0f,1.0f); 
			glNormal3d(0, 0, 1);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 85.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(55.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(55.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(85.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 115.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(85.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(85.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(115.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 145.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(115.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(115.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(145.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 145.0f, 480.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(115.0f, 480.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(115.0f, 450.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(145.0f, 450.0f, 0.2f);	// Bottom Right Of The Quad 
		glEnd();
	}
	else if (nextTetromino.type == SSHAPE)
	{
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);
		glBegin(GL_QUADS);		
		glColor3f(0.0f,1.0f,0.0f);
			glNormal3d(0, 0, 1);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 85.0f, 480.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(55.0f, 480.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(55.0f, 450.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(85.0f, 450.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 115.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(85.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(85.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(115.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 145.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(115.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(115.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(145.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 115.0f, 480.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(85.0f, 480.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(85.0f, 450.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(115.0f, 450.0f, 0.2f);	// Bottom Right Of The Quad 
		glEnd();
	}
	else if (nextTetromino.type == ZSHAPE)
	{
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);
		glBegin(GL_QUADS);
		glColor3f(1.0f,0.0f,0.0f);
			glNormal3d(0, 0, 1);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 85.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(55.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(55.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(85.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 115.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(85.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(85.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(115.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 145.0f, 480.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(115.0f, 480.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(115.0f, 450.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(145.0f, 450.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 115.0f, 480.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(85.0f, 480.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(85.0f, 450.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(115.0f, 450.0f, 0.2f);	// Bottom Right Of The Quad 
		glEnd();
	}
	else if (nextTetromino.type == TSHAPE)
	{
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);
		glBegin(GL_QUADS);
		glColor3f(0.63f,0.13f,0.94f);
			glNormal3d(0, 0, 1);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 85.0f, 480.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(55.0f, 480.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(55.0f, 450.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(85.0f, 450.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 115.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(85.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(85.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(115.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 145.0f, 480.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(115.0f, 480.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(115.0f, 450.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(145.0f, 450.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 115.0f, 480.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(85.0f, 480.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(85.0f, 450.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(115.0f, 450.0f, 0.2f);	// Bottom Right Of The Quad 
		glEnd();
	}
	else if (nextTetromino.type == OSHAPE)
	{
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);
		glBegin(GL_QUADS);
		glColor3f(1.0f,1.0f,0.0f);
			glNormal3d(0, 0, 1);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 100.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(70.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(70.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(100.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 130.0f, 510.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 510.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, 480.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(130.0f, 480.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 130.0f, 480.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 480.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, 450.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(130.0f, 450.0f, 0.2f);	// Bottom Right Of The Quad 

			glTexCoord2f(0.0f, 1.0f); glVertex3f( 100.0f, 480.0f, 0.2f);	// Top Right Of The Quad 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(70.0f, 480.0f, 0.2f);	// Top Left Of The Quad 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(70.0f, 450.0f, 0.2f);	// Bottom Left Of The Quad 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(100.0f, 450.0f, 0.2f);	// Bottom Right Of The Quad 
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
				
	// Restore the camera
	glLoadIdentity();
	gluPerspective( 90,1.0,0.1,100000 );
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();		// clear modelview
	glPushMatrix();			// save camera

	// setup camera, rotate it then move it away from board
	glTranslatef(0, BoardDistance, CameraDistance);
	glRotatef(CameraPitch, 1, 0, 0);
	glRotatef(CameraAngle, 0, 1, 0 );

	// draw bottom of board squares
	for(int x = 0; x < TetrisBoard.width; x++)
	{
		for(int y = 0; y < TetrisBoard.height; y++)
		{
			// save the camera matrix
			glPushMatrix();
			glTranslatef(2*x, 0, 2*y);
			glTranslatef(-TetrisBoard.width+1, 0, -TetrisBoard.height+1); // centers to middle of board
			glBegin(GL_LINE_LOOP);
			glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f( 1.0f,-1.0f, 1.0f);	// Top Right Of The Quad
				glVertex3f(-1.0f,-1.0f, 1.0f);	// Top Left Of The Quad
				glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad 
				glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad
			glEnd();
			// return the camera
			glPopMatrix();
		}
	}
	
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	// go through each section of the board and check if there is a block there.
	// if there is, then draw the block.
	for(int x = 0; x < TetrisBoard.width; x++)
	{
		for(int y = 0; y < TetrisBoard.height; y++)
		{
			for(int z = 0; z < TetrisBoard.depth; z++)
			{
				if(TetrisBoard.gameBoard[x][y][z].exists == true)
				{
					float red = TetrisBoard.gameBoard[x][y][z].red;
					float green = TetrisBoard.gameBoard[x][y][z].green;
					float blue = TetrisBoard.gameBoard[x][y][z].blue;
					// save the camera matrix
					glPushMatrix();

					glTranslatef( 2*x,2*z,2*y ); // translate to the spot
					glTranslatef(-TetrisBoard.width+1, 0, -TetrisBoard.height+1); // centers to middle of board
					// Drawing the cube
					glBindTexture(GL_TEXTURE_2D, texture[0].texID);
					glBegin(GL_QUADS);		// using quadrilaterals as primitive
					glColor3f(red,green,blue);
						glNormal3d(0, 1, 0);
						glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Top)
						glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Top)
						glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Top)
						glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Top)
					glColor3f(red,green,blue);
						glNormal3d(0, -1, 0);
						glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,-1.0f, 1.0f);	// Top Right Of The Quad (Bottom)
						glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,-1.0f, 1.0f);	// Top Left Of The Quad (Bottom)
						glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Bottom)
						glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Bottom)
					glColor3f(red,green,blue);
						glNormal3d(0, 0, 1);
						glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Front)
						glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Front)
						glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Front)
						glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Front)
					glColor3f(red,green,blue);
						glNormal3d(0, 0, -1);
						glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,-1.0f,-1.0f);	// Top Right Of The Quad (Back)
						glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,-1.0f,-1.0f);	// Top Left Of The Quad (Back)
						glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f,-1.0f);	// Bottom Left Of The Quad (Back)
						glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, 1.0f,-1.0f);	// Bottom Right Of The Quad (Back)
					glColor3f(red,green,blue);
						glNormal3d(-1, 0, 0);
						glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Left)
						glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Left)
						glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Left)
						glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Left)
					glColor3f(red,green,blue);
						glNormal3d(1, 0, 0);
						glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Right)
						glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Right)
						glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Right)
						glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Right)
					glEnd();
					// restore the camera matrix
					glPopMatrix();

				}
			}
		}
	}

	// Update the shadow Tetromino
	shadowTetromino = currentTetromino;

	// check if the piece will attach itself to the board (bottom level or any other piece)
	while(!shadowTetromino.ShadowDropOne())  // only true if it hits bottom
	{
		// do nothing, shadow drop increments by one until it hits bottom and returns true
	}

	// Draw the shadow tetromino
	for(int i = 0; i < 4; i++)
	{
		int x = shadowTetromino.blocks[i].x;
		int y = shadowTetromino.blocks[i].y;
		int z = shadowTetromino.blocks[i].z;

		// gray shadow color
		float red = 0.39;
		float green = 0.39;
		float blue = 0.39;
	
		// save the camera matrix
		glPushMatrix();

		glTranslatef( 2*x,2*z,2*y ); // translate to the spot
		glTranslatef(-TetrisBoard.width+1, 0, -TetrisBoard.height+1); // centers to middle of board
		// Drawing the cube
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);
		glBegin(GL_QUADS);		// using quadrilaterals as primitive
		glColor3f(red,green,blue);	// Color 
			glNormal3d(0, 1, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Top)
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Top)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Top)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Top)
		glColor3f(red,green,blue);	// Color 
			glNormal3d(0, -1, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,-1.0f, 1.0f);	// Top Right Of The Quad (Bottom)
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,-1.0f, 1.0f);	// Top Left Of The Quad (Bottom)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Bottom)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Bottom)
		glColor3f(red,green,blue);	// Color
			glNormal3d(0, 0, 1);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Front)
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Front)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Front)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Front)
		glColor3f(red,green,blue);	// Color 
			glNormal3d(0, 0, -1);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,-1.0f,-1.0f);	// Top Right Of The Quad (Back)
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,-1.0f,-1.0f);	// Top Left Of The Quad (Back)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f,-1.0f);	// Bottom Left Of The Quad (Back)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, 1.0f,-1.0f);	// Bottom Right Of The Quad (Back)
		glColor3f(red,green,blue);	// Color 
			glNormal3d(-1, 0, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Left)
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Left)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Left)
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Left)
		glColor3f(red,green,blue);	// Color
			glNormal3d(1, 0, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Right)
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Right)
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Right)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Right)

		glEnd();
		// restore the camera matrix
		glPopMatrix();
	}

	// draw the current tetromino
	for(int i = 0; i < 4; i++)
	{
		int x = currentTetromino.blocks[i].x;
		int y = currentTetromino.blocks[i].y;
		int z = currentTetromino.blocks[i].z;

		float red = currentTetromino.blocks[i].red;
		float green = currentTetromino.blocks[i].green;
		float blue = currentTetromino.blocks[i].blue;
	
		// save the camera matrix
		glPushMatrix();

		glTranslatef( 2*x,2*z,2*y ); // translate to the spot
		glTranslatef(-TetrisBoard.width+1, 0, -TetrisBoard.height+1); // centers to middle of board
		// Drawing the cube
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);
		glBegin(GL_QUADS);		// using quadrilaterals as primitive
		glColor3f(red,green,blue);	// Color 
			glNormal3d(0, 1, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Top)
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Top)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Top)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Top)
		glColor3f(red,green,blue);	// Color 
			glNormal3d(0, -1, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,-1.0f, 1.0f);	// Top Right Of The Quad (Bottom)
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,-1.0f, 1.0f);	// Top Left Of The Quad (Bottom)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Bottom)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Bottom)
		glColor3f(red,green,blue);	// Color
			glNormal3d(0, 0, 1);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Front)
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Front)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Front)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Front)
		glColor3f(red,green,blue);	// Color 
			glNormal3d(0, 0, -1);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,-1.0f,-1.0f);	// Top Right Of The Quad (Back)
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,-1.0f,-1.0f);	// Top Left Of The Quad (Back)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f,-1.0f);	// Bottom Left Of The Quad (Back)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, 1.0f,-1.0f);	// Bottom Right Of The Quad (Back)
		glColor3f(red,green,blue);	// Color 
			glNormal3d(-1, 0, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Left)
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Left)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Left)
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Left)
		glColor3f(red,green,blue);	// Color
			glNormal3d(1, 0, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Right)
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Right)
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Right)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Right)
		glEnd();

		// restore the camera matrix
		glPopMatrix();
	}

	// restore camera matrix	
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}

void processNormalKeys(unsigned char key, int x, int y) 
{
	//cout << (int)key << endl;
	if((int)key == 114) //r
	{
		if(CameraPitch == 70)
		{
			CameraPitch = 70;
		}
		else
		{
			CameraPitch += 10;
			CameraDistance -= 2;
			BoardDistance += 1;
		}
	}
	if((int)key == 102) //f
	{
		if(CameraPitch == 0)
		{
			CameraPitch = 0;
		}
		else
		{
			CameraPitch += -10;
			CameraDistance += 2;
			BoardDistance -=1;
		}
	}
	if((int)key == 99) //c
	{
		if(CameraAngle == 0)
			CameraAngle = 315;
		else
		{
			CameraAngle += -45;
		}
		
	}
	if((int)key == 118) //v
	{
		if(CameraAngle == 315)
			CameraAngle = 0;
		else
		{
			CameraAngle += 45;
		}
	}
	if(!gameover)
	{
		if((int)key == 120) //x
		{
			currentTetromino.RotateZCW();
		}
		if((int)key == 122) //z
		{
			currentTetromino.RotateZCCW();
		}
		if((int)key == 97) //a
		{
			if(CameraAngle == 0 || CameraAngle == 315)
			{
				currentTetromino.RotateYCW();
			}
			else if(CameraAngle == 90 || CameraAngle == 45)
			{
				currentTetromino.RotateXCW();
			}
			else if(CameraAngle == 180 || CameraAngle == 135)
			{
				currentTetromino.RotateYCCW();
			}
			else if(CameraAngle == 270 || CameraAngle == 225)
			{
				currentTetromino.RotateXCCW();
			}
		}
		if((int)key == 100) //d
		{
			if(CameraAngle == 0 || CameraAngle == 315)
			{
				currentTetromino.RotateYCCW();
			}
			else if(CameraAngle == 90 || CameraAngle == 45)
			{
				currentTetromino.RotateXCCW();
			}
			else if(CameraAngle == 180 || CameraAngle == 135)
			{
				currentTetromino.RotateYCW();
			}
			else if(CameraAngle == 270 || CameraAngle == 225)
			{
				currentTetromino.RotateXCW();
			}
			
		}
		if((int)key == 119) //w
		{
			if(CameraAngle == 0 || CameraAngle == 315)
			{
				currentTetromino.RotateXCW();
			}
			else if(CameraAngle == 90 || CameraAngle == 45)
			{
				currentTetromino.RotateYCCW();
			}
			else if(CameraAngle == 180 || CameraAngle == 135)
			{
				currentTetromino.RotateXCCW();
			}
			else if(CameraAngle == 270 || CameraAngle == 225)
			{
				currentTetromino.RotateYCW();
			}
			
		}
		if((int)key == 115) //s
		{
			if(CameraAngle == 0 || CameraAngle == 315)
			{
				currentTetromino.RotateXCCW();
				
			}
			else if(CameraAngle == 90 || CameraAngle == 45)
			{
				currentTetromino.RotateYCW();
			}
			else if(CameraAngle == 180 || CameraAngle == 135)
			{
				currentTetromino.RotateXCW();
				
			}
			else if(CameraAngle == 270 || CameraAngle == 225)
			{
				currentTetromino.RotateYCCW();
			}
			
		}
		if((int)key == 32) //space
		{
			currentTetromino = shadowTetromino;
			bool attached = currentTetromino.DropOne();
			if (attached)
			{
				attachedToBoard();
			}
		}
	}
	draw();
}

void processSpecialKeys(int key, int x, int y) {

	//cout << (int)key << endl;
	if(key == GLUT_KEY_UP)
	{
		if(CameraAngle == 0 || CameraAngle == 315)
		{
			currentTetromino.MoveBack();
		}
		else if(CameraAngle == 90 || CameraAngle == 45)
		{
			currentTetromino.MoveRight();
		}
		else if(CameraAngle == 180 || CameraAngle == 135)
		{
			currentTetromino.MoveForward();
		}
		else if(CameraAngle == 270 || CameraAngle == 225)
		{
			currentTetromino.MoveLeft();
		}
		
	}
	else if(key == GLUT_KEY_RIGHT)
	{
		if(CameraAngle == 0 || CameraAngle == 315)
		{
			currentTetromino.MoveRight();
		}
		else if(CameraAngle == 90 || CameraAngle == 45)
		{
			currentTetromino.MoveForward();
		}
		else if(CameraAngle == 180 || CameraAngle == 135)
		{
			currentTetromino.MoveLeft();
		}
		else if(CameraAngle == 270 || CameraAngle == 225)
		{
			currentTetromino.MoveBack();
		}
		
	}
	else if(key == GLUT_KEY_DOWN)
	{
		if(CameraAngle == 0 || CameraAngle == 315)
		{
			currentTetromino.MoveForward();
		}
		else if(CameraAngle == 90 || CameraAngle == 45)
		{
			currentTetromino.MoveLeft();
		}
		else if(CameraAngle == 180 || CameraAngle == 135)
		{
			currentTetromino.MoveBack();
		}
		else if(CameraAngle == 270 || CameraAngle == 225)
		{
			currentTetromino.MoveRight();
		}
		
	}
	else if(key == GLUT_KEY_LEFT)
	{
		if(CameraAngle == 0 || CameraAngle == 315)
		{
			currentTetromino.MoveLeft();
		}
		else if(CameraAngle == 90 || CameraAngle == 45)
		{
			currentTetromino.MoveBack();
		}
		else if(CameraAngle == 180 || CameraAngle == 135)
		{
			currentTetromino.MoveRight();
		}
		else if(CameraAngle == 270 || CameraAngle == 225)
		{
			currentTetromino.MoveForward();
		}
		
	}
	else if(key == GLUT_KEY_F2)
	{
		// Reset the game
		gameover = false;
		level = 1;
		lines = 0;
		score = 0;
		TimeToDrop = 5000;
		Tetromino newCurrentTetromino;
		Tetromino newNextTetromino;
		currentTetromino = newCurrentTetromino;
		nextTetromino = newNextTetromino;
		TetrisBoard.clearBoard();

	}
	draw();
}

void IncrementGameTime( int value )
{
	// check if it was attached to the board after dropping by one
	bool attached = currentTetromino.DropOne();
	if (attached)
	{
		attachedToBoard();
	}
	glutPostRedisplay();
    glutTimerFunc(TimeToDrop, IncrementGameTime, 1);
}

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( 800,600 );
	glutCreateWindow( "3D Tetris --- Bryan Denny" );
	glutDisplayFunc(draw);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutTimerFunc(TimeToDrop, IncrementGameTime, 1 );
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	//fixes some depth issues

	// enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Create light components
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	//GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };

	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	// Enable/Load textures
	glEnable( GL_TEXTURE_2D );
	LoadGLTextures();
	glutMainLoop();
}


