#ifndef _TETRIS_H_
#define _TETRIS_H_


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <string.h>

#define WIDTH	10
#define HEIGHT	22
#define NOTHING	0
#define QUIT	'q'
#define NUM_OF_SHAPE	7
#define NUM_OF_ROTATE	4
#define BLOCK_HEIGHT	4
#define BLOCK_WIDTH	4
#define BLOCK_NUM	3

// menu number
#define MENU_PLAY '1'
#define MENU_RANK '2'
#define MENU_RECO '3'
#define MENU_EXIT '4'

// User Name
#define NAMELEN 16

#define CHILDREN_MAX 36

#define VISIBLE_BLOCKS 3

typedef struct _RecNode{
	int lv,score;
	char (*f)[WIDTH];
	struct _RecNode *c[CHILDREN_MAX];
} RecNode;

/* [blockShapeID][# of rotate][][]*/
const char block[NUM_OF_SHAPE][NUM_OF_ROTATE][BLOCK_HEIGHT][BLOCK_WIDTH] ={
	{/*[0][][][]					▩▩▩▩*/
		{/*[][0][][]*/
			{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}
		},
		{/*[][1][][]*/
			{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}			
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}			
		},
		{/*[][3][][]*/
			{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}
		}
	},
	{/*[1][][][];					  ▩▩▩*/
		{/*[][0][][]				      ▩*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0 ,1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}
		}
	},
	{/*[2][][][];					  ▩▩▩*/
		{/*[][0][][]				  ▩*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}
		}
	},
	{/*[3][][][];					  ▩▩▩*/
		{/*[][0][][]				    ▩*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}
		}
	},
	{/*[4][][][];					  ▩▩*/
		{/*[][0][][]				  ▩▩*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		}
	},
	{/*[5][][][];					  ▩▩*/
		{/*[][0][][]				▩▩*/
			{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}			
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}			
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}			
		}
	},
	{/*[6][][][];					▩▩*/
		{/*[][0][][]				  ▩▩*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}
		}
	}
};

char field[HEIGHT][WIDTH];	/* Tetris main */
int nextBlock[BLOCK_NUM];	/* Block ID array; [0]: current block; [1]: next block; [2]: next next block */
int blockRotate,blockY,blockX;	/* rotated block, block's Y coordinate, block's X coordinate */
int score;			/* score */
int gameOver = 0;			/* If the game ended, set 1 */
int timed_out;
int recommendR,recommendY,recommendX; /* Recommended block rotation, Y coordinate, X coordinate */
RecNode *recRoot;

/***********************************************************
 *	Initialise all global variables of Tetris.
 *	input	: none
 *	return	: none
 ***********************************************************/
void InitTetris();

/***********************************************************
 *  Interface of Tetris	
 *	input	: none
 *	return	: none
 ***********************************************************/
void DrawOutline();

/***********************************************************
 *	Keyboard input handler
 *	input	: none
 *	return	: (int) command
 *		  KEY_DOWN : down
 *		  KEY_UP   : up
 *		  KEY_RIGHT: right
 *		  KEY_LEFT : left
 *		  ' '	   : Space bar
 *		  'q'/'Q'  : quit
 ***********************************************************/
int GetCommand();

/***********************************************************
 *	Process Command with the given command
 *	input	: (int) GetCommand로 받은 command
 *	return	: (int) quit; if command is quit return 0,
 *		                  else return 1
 ***********************************************************/
int ProcessCommand(int command);

/***********************************************************
 *  Block down for 1 second
 *		- If the block can not go down anymore,
 *		- Add the block to the field.
 *		- Delete the line that is completely filled.
 *		- Change the next block to the current block
 *		- Reset the block's coordinates.
 *		- Draw the next block and update the score.
 *	input	: (int) sig
 *	return	: none
 ***********************************************************/
void BlockDown(int sig);

/***********************************************************
 *  Check if the block can move
 *  input	: (char[][]) field to check the block's movement
 *		  (int) current block's shape ID
 *		  (int) block's rotation count
 *		  (int) block's Y coordinate
 *		  (int) block's X coordinate
 *  return	: (int) if the block can move, return 1
 *		  if not, return 0
 ***********************************************************/
int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX);

/***********************************************************
 *	Redraw the changed part of the field
 *  input	: (char[][]) field to check the block's movement
 *		  (int) command to check the block's movement
 *		  (int) current block's shape ID
 *		  (int) block's rotation count
 *		  (int) block's Y coordinate
 *		  (int) block's X coordinate
 *	return	: none
 ***********************************************************/
void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX);

/***********************************************************
 *	Draw the field that the block is falling
 *	input	: none
 *	return	: none
 ***********************************************************/
void DrawField();

/***********************************************************
 *  Add the falling block to the field	
 *  input	: (char[][]) field to check the block's movement
 * 		  (int) current block's shape ID
 * 		  (int) block's rotation count
 * 		  (int) block's Y coordinate
 * 		  (int) block's X coordinate
 *	return	: none
 ***********************************************************/
int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX);

/***********************************************************
 *	Delete the line that is completely filled and return the score.
 *  input	: (char[][]) field to check the block's movement
 *  return	: (int) the score of the deleted lines
 ***********************************************************/
int DeleteLine(char f[HEIGHT][WIDTH]);

/***********************************************************
 *	Move the cursor to the given y, x coordinate
 *	input	: (int) y coordinate
 *		  (int) x coordinate
 *	return	: none
 ***********************************************************/ 
void gotoyx(int y, int x);

/***********************************************************
 *	Draw the next block in the upper right corner of the Tetris screen.
 *  input	: (int*) array of block shape ID
 *  return	: none
 ***********************************************************/
void DrawNextBlock(int *nextBlock);

/***********************************************************
 *	Print the score in the bottom right corner of the Tetris screen.
 *  input	: (int) score to print
 *  return	: none
 ***********************************************************/
void PrintScore(int score);

/***********************************************************
 *	Draw the box in the given y, x coordinate with the given height and width.
 *  input	: (int) y coordinate of the top left corner
 * 		 (int) x coordinate of the top left corner
 *		 (int) height of the box
 *		 (int) width of the box
 *	return	: none
 ***********************************************************/
void DrawBox(int y,int x, int height, int width);

/***********************************************************
 *	Draw the given block shape in the given y, x coordinate.
 *  input	: (int) y coordinate of the top left corner
 * 		 (int) x coordinate of the top left corner
 *		 (int) block shape ID
 *		 (int) block rotation count
 *		 (char) block pattern to draw
 *	return	: none
 ***********************************************************/
void DrawBlock(int y, int x, int blockID,int blockRotate,char tile);

/***********************************************************
 *	Show the shadow of the block in the given y, x coordinate.
 *  input	: (int) y coordinate of the top left corner
 *		 (int) x coordinate of the top left corner
 *		 (int) block shape ID
 *		 (int) block rotation count
 *	return	: none
 ***********************************************************/
void DrawShadow(int y, int x, int blockID,int blockRotate);

/***********************************************************
 *	Start the game
 *	input	: none
 *	return	: none
 ***********************************************************/
void play();

/***********************************************************
 *	Show the menu and get the user's choice.
 *	input	: none
 *	return	: user's choice
 ***********************************************************/
char menu();

/***********************************************************
 *	read the rank file and create the rank list.
 *	input	: none
 *	return	: none
 ***********************************************************/
void createRankList();

/***********************************************************
 *	show the rank list.
 *	input	: none
 *	return	: none
 ***********************************************************/
void rank();

/***********************************************************
 *	make a new rank file.
 *	input	: none
 *	return	: none
 ***********************************************************/
void writeRankFile();

/***********************************************************
 *	add a new rank to the rank list.
 *	input	: (int) new score
 *	return	: none
 ***********************************************************/
void newRank(int score);

/***********************************************************
 *  Recommend the block to be placed in the given field.
 *	input	: (RecNode*) Recommendation tree's root
 *	return	: (int) the expected score obtained by following the recommended block placement	
 ***********************************************************/
int recommend(RecNode *root);

/***********************************************************
 *	Play the game with the recommended block placement.
 *	input	: none
 *	return	: none
 ***********************************************************/
void recommendedPlay();

void DrawBlockWithFeatures(int y, int x, int blockID,  int blockRotate);
void DrawRecommend(int y, int x, int blockID,int blockRotate);
void BlockDownRecommend(int sig);
int modified_recommend(char rfield[HEIGHT][WIDTH], int lv, int score);

void constructRecTree(RecNode *root);

int evalState(int lv, char f[HEIGHT][WIDTH], int r, int y, int x);

#endif
