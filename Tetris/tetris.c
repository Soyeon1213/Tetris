#include "tetris.h"

static struct sigaction act, oact;

typedef struct _node {
    char name[NAMELEN];
    int score;
    struct _node *next;
} NODE;

RecNode *recRoot;

int rank_count = 0;
NODE *rankH = NULL;
NODE *rankT = NULL;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

    recRoot = (RecNode*)malloc(sizeof(RecNode));
    recRoot->lv=-1;
    recRoot->score=0;
    recRoot->f=field;
    constructRecTree(recRoot);

    createRankList();
	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
        case MENU_RANK: rank(); break;
        case MENU_RECO: recommendedPlay(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

    writeRankFile();

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;
/*
	for ( i = 0 ; i < BLOCK_NUM ; i++ ) {
        nextBlock[i] = rand()%7;
    }
*/
    nextBlock[0] = rand()%7;
    nextBlock[1] = rand()%7;
    nextBlock[2] = rand()%7;

    // recommend(recRoot);
    modified_recommend(field, 0, 0);

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	// DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
    //DrawRecommend(blockY, blockX, nextBlock[0], blockRotate);
    DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;

    // Draw the outline of the falling block area
	DrawBox(0,0,HEIGHT,WIDTH);

	// Draw the outline of the next block area
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
    
    move(9,WIDTH+10);
	DrawBox(10,WIDTH+10,4,8);
	
    // Draw the outline of the score area
	move(17,WIDTH+10);
	printw("SCORE");
	DrawBox(18,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(19,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
    }
    for( i = 0 ; i < 4 ; i++ ) {
        move(11+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++) {
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}
    }
	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    int i, j;
    for ( i = 0 ; i < 4 ; i++ ) {
        for ( j = 0 ; j < 4 ; j++ ) {
            if(block[currentBlock][blockRotate][i][j] == 1){
                if(f[blockY+i][blockX+j]) return 0;
                if(blockY+i>=HEIGHT) return 0;
                if(blockX+j<0) return 0;
                if(blockX+j>=WIDTH) return 0;
            }
        }
    }
    return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    int i, j, curBlock = currentBlock, curRotate = blockRotate, curY = blockY, curX = blockX, oldShadowY;
    switch ( command ) {
        case KEY_UP :  curRotate = (curRotate+3)%4;
                      break;
        case KEY_DOWN : curY--;
                        break;
        case KEY_LEFT : curX++;
                        break;
        case KEY_RIGHT : curX--;
                         break;
    }
    oldShadowY = curY;
    j = oldShadowY;
    for ( i = oldShadowY ; i < HEIGHT-1 ; i++ ) {
        if ( CheckToMove(f, curBlock, curRotate, i, curX) == 1 ) {
            j++;
        }
        else {
            break;
        }
    }
    
    oldShadowY = j-1;

    for ( i = 0 ; i < 4 ; i++ ) {
        for ( j = 0 ; j < 4 ; j++ ) {
            if ( block[curBlock][curRotate][i][j] == 1 ) {
                if ( i+curY >= 0 ) {
                    move(i+curY+1, j+curX+1);
                    printw(".");
                }
                if ( i+oldShadowY >= 0 ) {
                    move(i+oldShadowY+1, j+curX+1);
                    printw(".");
                }
            }
        }
    }
    // DrawBlock( blockY, blockX, currentBlock, blockRotate, ' ');
    DrawBlockWithFeatures( blockY, blockX, currentBlock, blockRotate);
    move(HEIGHT, WIDTH+10);
}

void BlockDown(int sig){
	// user code
    int i;
    if ( CheckToMove( field, nextBlock[0], blockRotate, blockY+1, blockX) == 1 ) {
        blockY++;
        DrawChange( field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
    }
    else {
        // AddBlockToField( field, nextBlock[0], blockRotate, blockY, blockX);
        if ( blockY == -1 ) {
            gameOver = 1;
        }
        else {
            score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
            score += DeleteLine(field);
            nextBlock[0] = nextBlock[1];
            nextBlock[1] = nextBlock[2];
            nextBlock[2] = rand()%7;
            //recommend(recRoot);
            modified_recommend(field, 0, 0);
            DrawNextBlock(nextBlock);
            blockRotate=0;
	        blockY=-1;
	        blockX=WIDTH/2-2;
            PrintScore(score);
        }
        DrawField();
    }
    // DrawChange( field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
    timed_out = 0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    int i, j;
    int touched = 0;
    for ( i = 0 ; i < 4  ; i++ ) {
        for ( j = 0 ; j < 4 ; j++ ) {
            if ( block[currentBlock][blockRotate][i][j] == 1 ) {
                f[blockY+i][blockX+j] = 1;
                if ( (blockY+i+1) == HEIGHT || f[blockY+i+1][blockX+j] == 1 ) {
                        ++touched;
                }   
            }
        }
    }
    return touched * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
    int i, j, k ;
    int count = 0;
    int full;
    for ( i = 1 ; i < HEIGHT ; i++ ) {
        full = 1;
        for ( j = 0 ; j < WIDTH ; j++ ) {
            if ( f[i][j] == 0 ) {
                full = 0;
                break;
            }
        }
        if ( full == 1 ) {
            count++;
            for ( k = 1 ; k < i ; k++ ) {
                for ( j = 1 ; j < WIDTH ; j++ ) {
                    f[i-k+1][j] = f[i-k][j];
                }
            }
        }
    }
    return count*count*100;
}

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
    int i, j = y;
    for( i = y ; i < HEIGHT ; i++ ) {
        if ( CheckToMove(field, blockID, blockRotate, i, x) == 1 ) {
            j++;
        }
        else {
            break;
        }
    }
    DrawBlock(j-1, x, blockID, blockRotate,'/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) { 
    DrawRecommend(y, x, blockID, blockRotate);
    DrawShadow(y, x, blockID, blockRotate);
    DrawBlock(y, x, blockID, blockRotate, ' ');
}

void createRankList(){
	// user code
    int i;
    FILE* fp;
    fp = fopen("rank.txt", "r");
    if( (fp = fopen("rank.txt", "r")) == NULL ) {
        return;
    }
  
    fscanf(fp, "%d", &rank_count);
    for ( i = 0 ;i < rank_count ; i++ ) {
        NODE *temp;
        temp = (NODE*)malloc(sizeof(NODE));
        fscanf(fp, "%s %d", temp->name, &temp->score);
        temp->next = NULL;
        if( i == 0 ) {
            rankH = temp;
            rankT = temp;
        }
        else {
            rankT->next = temp;
            rankT = temp;
        }   
    }
    fclose(fp);
}

void rank(){
	// user code
    int from = 0, to = 0, i;
    char name[NAMELEN];
    int del;
    clear();
    printw("1. list ranks from X to Y\n");
    printw("2. list ranks by a specific name\n");
    printw("3. delete a specific rank\n");
    switch(wgetch(stdscr)) {
        case '1':
            echo();
            printw("\n");
            printw("X: ");
            scanw("%d", &from);
            printw("Y: ");
            scanw("%d", &to);
            noecho();
            if ( from == 0 ) {
                from = 1;
            }
            if ( to == 0 ) {
                to = rank_count;
            }
            printw("\n");
            printw("      name      |    score    \n");
            printw("------------------------------\n");
            if ( ( from <= to ) && ( from >= 1 ) && ( to <= rank_count ) ) {
                NODE *ptr = rankH;
                for ( i = 1 ; i < from ; i++ ) {
                    ptr = ptr->next;
                }
                for ( i = from ; i <= to ; i++ ) {
                    printw("%15s | %d\n", ptr->name, ptr->score);
                    ptr = ptr->next;
                }
            }
            else {
                printw("\nsearch failure: no rank in the list\n");
            }

            break;
        case '2':
            printw("input the name: ");
            echo();
            getstr(name);
            noecho();
            printw("      name      |    score    \n");
            printw("------------------------------\n");

            NODE *ptr2 = rankH;
            int a = 0;
            while(1) {
                if( strcmp(ptr2->name, name) == 0 ) {
                    a = 1;
                    printw("%15s | %d\n", ptr2->name, ptr2->score);
                }
                if( ptr2->next == NULL ) {
                    break;
                }
                ptr2=ptr2->next;
            }
            if( a == 0 ) {
                printw("\nsearch failure: no name in the list\n");
            }

            break;
       
        case '3':
            printw("input the rank: ");
            echo();
            scanw("%d", &del);
            noecho();

            NODE *before;
            NODE *cur = rankH;
            int c = 1;

            if( rank_count >= del && del > 0 ) {
                if( del == 1 ) {
                    rankH=rankH->next;
                    free(cur);
                    rank_count--;
                    printw("result: the rank deleted\n");
                }
                else { 
                    while( c < del ) {
                        before = cur;
                        cur = cur->next;
                        c++;
                    }
                    before->next = cur->next;
                    free(cur);
                    rank_count--;
                    printw("result: the rank deleted\n");
                }
            }
            else {
                printw("search failure: the rank not in the list\n");
            }
            break;
         
        default : break;
    }
    getch();
    
}

void writeRankFile(){
	// user code
    int i;
    FILE* fp;
    fp = fopen("rank.txt", "w");
    fprintf(fp, "%d\n", rank_count);

    NODE *ptr = rankH;
    for ( i = 0 ; i < rank_count ; i++ ) {
        fprintf(fp, "%s %d\n", ptr->name, ptr->score);
        ptr = ptr->next;
    }
    
    fclose(fp);
}

void newRank(int score){
	// user code
    int i, a;
    char nameTemp[NAMELEN];
    clear();
    printw("your name: ");
    echo();
    getstr(nameTemp);
 
    NODE *new;
    new = (NODE*)malloc(sizeof(NODE));
    strcpy(new->name, nameTemp);
    new->score = score;
    new->next = NULL;
    rank_count++;

    NODE *before = rankH;
    NODE *cur = rankH;

    if ( rank_count == 1 ) {
        rankH = new;
    }
    else if ( ( new->score ) > ( rankH->score ) ) {
            new->next = rankH;
            rankH = new;
    }
    else {
        while(cur) {
            if ( ( new->score ) > ( cur->score ) ) {
                before->next = new;
                new->next = cur;
                break;
            }
            before = cur;
            cur = cur->next;
        }
        before->next = new;
    }
}

void constructRecTree(RecNode *root) {
    int i, h;
    RecNode **c=root->c;
    for( i = 0 ; i < CHILDREN_MAX ; i++ ) {
        c[i] = (RecNode*)malloc(sizeof(RecNode));
        c[i]->lv = root->lv+1;
        c[i]->f=(char(*)[WIDTH])malloc(sizeof(char)*HEIGHT*WIDTH);
        if( c[i]->lv < VISIBLE_BLOCKS ) {
            constructRecTree(c[i]);
        }
    }
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
    if(CheckToMove(field, nextBlock[0], recommendR, recommendY, recommendX) == 1 )
        DrawBlock(recommendY, recommendX, nextBlock[0], recommendR, 'R');
}

int recommend(RecNode *root){
	int r, x, y, rBoundary, lBoundary;
    int h, w;
    int eval;
    int max=0; // maximum score with the recommended block
    int solR, solY, solX;
    int i = 0;
    
    int lv = root->lv+1;
 
    RecNode **c = root->c;
    int recommended = 0;

    for( r = 0 ; r < NUM_OF_ROTATE ; ++r ) {
        lBoundary = 3;
        for ( h = 0 ; h < BLOCK_HEIGHT ; ++h ) {
            for ( w = 0 ; w < BLOCK_WIDTH ; ++w ) {
                if ( block[nextBlock[lv]][r][h][w]) {
                    break;
                }
            }
            if ( w < lBoundary ) {
                lBoundary = w;
            }
        }
        lBoundary = 0-lBoundary;

        rBoundary = 0;
        for( h = 0 ; h < BLOCK_HEIGHT ; ++h ) {
            for( w = BLOCK_WIDTH-1 ; w >= 0 ; --w ) {
                if ( block[nextBlock[lv]][r][h][w] ) {
                    break;
                }
            }
            if(w>rBoundary) {
                rBoundary = w;
            }
        }
        rBoundary = WIDTH-1-rBoundary;

        for ( x = lBoundary ; x <= rBoundary ; ++x, ++i ) {
            for ( h = 0 ; h < HEIGHT ; ++h ) {
                for ( w = 0 ; w < WIDTH ; ++w ) {
                    c[i]->f[h][w] = root->f[h][w];
                }
            }
            y = 0;
            if(CheckToMove(c[i]->f,nextBlock[lv],r,y,x)){
                while(CheckToMove(c[i]->f,nextBlock[lv],r,++y,x));
                --y;
            }
            else { 
                continue;
            }

            c[i]->score = root->score+evalState(lv, c[i]->f, r, y, x);

            if( lv < VISIBLE_BLOCKS-1 ) {
                eval = recommend(c[i]);
            }
            else {
                eval = c[i]->score;
            }
            if( max<eval ) {
                recommended=1;
                max = eval;
                solR = r;
                solY = y;
                solX = x;
            }
        }
    }
   
    if ( lv == 0 && recommended ) {
        recommendR = solR;
        recommendY = solY;
        recommendX = solX;
        
    }

	return max;
}

int evalState(int lv, char f[HEIGHT][WIDTH], int r, int y, int x) {
    return AddBlockToField(f, nextBlock[lv], r, y, x) + DeleteLine(f);
}

void BlockDownRecommend(int sig) {
    int i;
            
    if ( !CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX) ) {
        gameOver = 1;
    }
    else {
        score += AddBlockToField(field, nextBlock[0], recommendR, recommendY, recommendX);
            score += DeleteLine(field);
            blockRotate=0;
	        blockY=-1;
	        blockX=WIDTH/2-2;
            for ( i = 0 ; i < VISIBLE_BLOCKS-1 ; i++ ) {
                nextBlock[i] = nextBlock[i+1];
            }
            nextBlock[VISIBLE_BLOCKS-1] = rand()%7;
            // recommend(recRoot);
            modified_recommend(field, 0, 0);
            DrawField();
            DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
            DrawNextBlock(nextBlock);
            PrintScore(score);
            timed_out = 0;
    }
}

int modified_recommend(char rfield[HEIGHT][WIDTH], int lv, int score) {
    char fieldNew[HEIGHT][WIDTH];
    int r, x, y;
    int rBoundary, lBoundary;
    int i, j, h, w;
    int eval;
    int max = 0;
    int solR, solY, solX;
    int recommended = 0;

    if ( lv > VISIBLE_BLOCKS-1 ) {
        return score;
    }

    for ( r = 0 ; r < NUM_OF_ROTATE ; r++ ) {
        lBoundary = 3;
        for ( h = 0 ; h < BLOCK_HEIGHT ; ++h ) {
            for ( w = 0 ; w < BLOCK_WIDTH ; ++w ) {
                if ( block[nextBlock[lv]][r][h][w] ) {
                    break;
                }
            }
            if ( w < lBoundary ) {
                lBoundary = w;
            }
        }
        lBoundary = 0 - lBoundary;
        rBoundary = 0;
        for ( h = 0 ; h < BLOCK_HEIGHT ; ++h ) {
            for ( w = BLOCK_WIDTH-1 ; w >= 0 ; --w ) {
                if ( block[nextBlock[lv]][r][h][w] ) {
                    break;
                }
            }
            if ( w > rBoundary ) {
                rBoundary = w;
            }
        }
        rBoundary = WIDTH-1-rBoundary;

        for ( x = lBoundary ; x <= rBoundary ; x++ ) {
            for ( i = 0 ; i < HEIGHT ; ++i ) {
                for ( j = 0 ; j < WIDTH ; ++j ) {
                    fieldNew[i][j] = rfield[i][j];
                }
            }
            y = 0;
            if ( CheckToMove(fieldNew, nextBlock[lv], r, y, x) ) {
                while ( CheckToMove(fieldNew, nextBlock[lv], r, ++y, x) );
                --y;
            }
            else {
                continue;
            }

            eval = modified_recommend(fieldNew, lv+1, score+evalState(lv, fieldNew, r, y, x));

            if ( max < eval ) {
                recommended = 1;
                max = eval;
                solR = r;
                solY = y;
                solX = x;
            }
        }
    }

    if ( lv == 0 && recommended ) {
        recommendR = solR;
        recommendY = solY;
        recommendX = solX;
        
    }

	return max;
}

void recommendedPlay(){
    int command;
    clear();
    act.sa_handler = BlockDownRecommend;
    sigaction(SIGALRM, &act, &oact);
    InitTetris();
    do { 
        if ( timed_out == 0 ) {
            alarm(1);
            timed_out = 1;
        }
        command = GetCommand();
        if(command=='q' || command=='Q'){
            alarm(0);
            DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
            move(HEIGHT/2,WIDTH/2-4);
            printw("Good-bye!!");
            refresh();
            getch();
            return;
        }
    } while(!gameOver);
    alarm(0);
    getch();
    DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
    move(HEIGHT/2,WIDTH/2-4);
    printw("GameOver!!");
    refresh();
    getch();
    newRank(score);
}
