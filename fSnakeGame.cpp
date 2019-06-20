#include "fSnakeGame.h"
#include <unistd.h>

using namespace std;

// Unreal coding standards
using int32 = int;

CharPosition::CharPosition(int32 col, int32 row)
{
	x = col;
	y = row;
}

CharPosition::CharPosition()
{
	x = 0;
	y = 0;
}

fSnakeGame::fSnakeGame()
{
	// variables initialisation:
	partchar = 'x'; // character to represent the snake
	edgechar = (char)219; // full rectangle on the key table
	fruitchar = '+';
	opponentChar = '@';
	fruit.x = 0;
	fruit.y = 0;
	score = 0;
	del = 110000;
	bool bEatsFruit = 0;
	endGame = false;
	direction = 'l';
	srand(time(NULL));
	numberOfOpponents = 0;
	numberOfStartOpponents =3;
	maxNumberOfOpponents = 10;
	opponentWaitTime =2;
	opponentKillTime = 60;

	opponentsThreads =  new std::thread[this->maxNumberOfOpponents];

	InitGameWindow();
	PositionFruit();
	DrawWindow();
	DrawSnake();
	PrintScore();
	initThreads();
    //refresh();

	/*
	for(int i=0; i < numberOfOpponents; i++)
        opponentsThreads->join();
*/
    refresh();

}

fSnakeGame::~fSnakeGame()
{
    snakeThread->join();
    timeThread->join();
    for(int i=0; i < numberOfOpponents; i++)
	    opponentsThreads->join();


	//nodelay(stdscr, false);
	//getch();
	//endwin();
}

// initialise the game window
void fSnakeGame::InitGameWindow() 
{ 
	initscr(); // initialise the screen
	nodelay(stdscr,TRUE);
	keypad(stdscr, true); // initialise the keyboard: we can use arrows for directions
	noecho(); // user input is not displayed on the screen
	curs_set(0); // cursor symbol is not not displayed on the screen (Linux)
	getmaxyx(stdscr, maxheight, maxwidth); // define dimensions of game window
    use_default_colors();
    start_color();
	return; 
}

// draw the game window
void fSnakeGame::DrawWindow()
{
	for (int32 i = 0; i < maxwidth; i++) // draws top
	{
		move(0, i);
		addch(edgechar);
	}

	for (int32 i = 0; i < maxwidth; i++) // draws bottom
	{
		move(maxheight-2, i);
		addch(edgechar);
	}

	for (int32 i = 0; i < maxheight-1; i++) // draws left side
	{
		move(i, 0);
		addch(edgechar);
	}

	for (int32 i = 0; i < maxheight-1; i++) // draws right side
	{
		move(i, maxwidth-1);
		addch(edgechar);
	}
	return;
}

// draw snake's body
void fSnakeGame::DrawSnake()
{
	for (int32 i = 0; i < 5; i++) 
	{
		snake.push_back(CharPosition(30+i, 10));
	}

	for (int32 i = 0; i < snake.size(); i++)
	{
        this->Drawing(snake[i].x, snake[i].y, partchar, -1,-1);
	}
	return;
}

// print score at bottom of window
void fSnakeGame::PrintScore()
{
	move(maxheight-1, 0);
	printw("Score: %d", score);
	return;
}

// position a new fruit in the game window
void fSnakeGame::PositionFruit()
{
	while(1)
	{
		int32 tmpx = rand()%maxwidth+1; // +1 to avoid the 0
		int32 tmpy = rand()%maxheight+1;

		// check that the fruit is not positioned on the snake
		for (int32 i = 0; i < snake.size(); i++)
		{
			if (snake[i].x == tmpx && snake[i].y == tmpy)
			{
				continue; // if true, ignore the following and go back to the beginning of function
			}
		}

		// check that the fruit is positioned within the game window
		if (tmpx >= maxwidth-2 || tmpy >= maxheight-3)
		{
			continue; // if true, ignore the following and go back to the beginning of function
		}

		// if the coordinates are valid, add fruit in the window
		fruit.x = tmpx;
		fruit.y = tmpy;
		break;
	}

    this->Drawing(fruit.x, fruit.y, fruitchar, -1,-1);
}

// set game over situations
bool fSnakeGame::FatalCollision()
{
	// if the snake hits the edge of the window
	if (snake[0].x == 0 || snake[0].x == maxwidth-1 || snake[0].y == 0 || snake[0].y == maxheight-2)
	{
		return true;
	}

	// if the snake collides into himself
	for (int32 i = 2; i < snake.size(); i++)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
		{
			return true;
		}
	}

	return false;
}

// define behaviour when snake eats the fruit
bool fSnakeGame::GetsFruit()
{
	if (snake[0].x == fruit.x && snake[0].y == fruit.y)
	{
		PositionFruit(); 
		score +=10; 
		PrintScore();
		if(score%40 ==0)this->AddOpponent();
		// if score is a multiple of 100, increase snake speed
		if ((score%100) == 0)
		{
			del -= 1000;
		}
		return bEatsFruit = true;
	}
	else 
	{
		return bEatsFruit = false;
	}
	return bEatsFruit;
}

// define snake's movements
void fSnakeGame::MoveSnake()
{
	int32 KeyPressed = getch();
	switch(KeyPressed)
	{
		case KEY_LEFT:
			if (direction != 'r') 
			{ direction = 'l'; }  
			break;
		case KEY_RIGHT:
			if (direction != 'l')
			{ direction = 'r'; }
			break;
		case KEY_UP:
			if (direction != 'd')
			{ direction = 'u'; }
			break;
		case KEY_DOWN:
			if (direction != 'u')
			{ direction = 'd'; }
			break;
		case KEY_BACKSPACE:
			direction = 'q'; // key to quit the game
			break;
	}

	// the snake doesn't eat fruit, remains same size
	if (!bEatsFruit)
	{
        this->Drawing(snake[snake.size()-1].x, snake[snake.size()-1].y, ' ', -1,-1);
		snake.pop_back(); // removes the last element in the vector, reducing the container size by one
	}

	// the snake moves and we add an extra character at the beginning of the vector
	// add a head and initialise new coordinates for CharPosition according to the direction input
	if (direction == 'l')
	{ snake.insert(snake.begin(), CharPosition(snake[0].x-1, snake[0].y)); } 
	else if (direction == 'r')
	{ snake.insert(snake.begin(), CharPosition(snake[0].x+1, snake[0].y)); }
	else if (direction == 'u')
	{ snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y-1)); }
	else if (direction == 'd')
	{ snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y+1)); }


	this->Drawing(snake[0].x, snake[0].y, partchar, -1,-1);
	return;
}

void fSnakeGame::PlayGame()
{
    while(1)
    {
        if (FatalCollision())
        {
            move((maxheight-2)/2,(maxwidth-5)/2);
            printw("Out of Board! GAME OVER");
            endGame = true;
            //timeThread->join();
            //for(int i=0; i < numberOfOpponents; i++)
               //opponentsThreads->join();
            //this->EndGame();
            break;
        }

        if(CollisionWithOpponent()){
            move((maxheight-2)/2,(maxwidth-5)/2);
            printw("Opponent collision! GAME OVER");
			endGame = true;
			//timeThread->join();
            //for(int i=0; i < numberOfOpponents; i++)
                //opponentsThreads->join();
            //this->EndGame();
            break;

        }
        GetsFruit();
        MoveSnake();

        if (direction=='q') //exit
        {
			endGame = true;
            this->EndGame();
        	break;
        }

        usleep(del); // delay
    }
    //for(int i=0; i < opponentsThreads.; i++)
    //opponentsThreads->join();

    //snakeThread->join();

    //snakeThread->join();
}

void fSnakeGame::MoveOpponent(int id) {

	while (endGame!= true){
	    int oldX = opponents[id].x;
	    int oldY = opponents[id].y;
		int moveOpp = rand()%4;
		switch (moveOpp){
			case 0:{
			    if(opponents[id].x-1 >0) {
                    opponents[id].x--;
                }
                break;

			}
			case 1:{
			    if(opponents[id].x+1 <maxwidth-1) {
                    opponents[id].x++;
                }
			    break;
			}
			case 2:{
			    if(opponents[id].y-1 >0) {
                    opponents[id].y--;
                }break;

			}
			case 3: {
                if (opponents[id].y + 1 < maxheight - 2) {
                    opponents[id].y++;
                }
                break;
            }
		}
        this->Drawing(opponents[id].x, opponents[id].y, opponentChar, oldX,oldY);
		sleep(opponentWaitTime);
        bool opponentFruit =  OpponentEatsFruit(id);
	}

}

void fSnakeGame::AddOpponent() {

	if(numberOfOpponents < maxNumberOfOpponents) {
		CharPosition opponent = CharPosition();
		opponent.x = 0;
		opponent.y = 0;

		while (1) {
			int32 tmpx = rand() % maxwidth + 1; // +1 to avoid the 0
			int32 tmpy = rand() % maxheight + 1;

			// check that the fruit is not positioned on the snake
			for (int32 i = 0; i < snake.size(); i++) {
				if (snake[i].x == tmpx && snake[i].y == tmpy) {
					continue; // if true, ignore the following and go back to the beginning of function
				}
			}

			// check that the fruit is positioned within the game window
			if (tmpx >= maxwidth - 2 || tmpy >= maxheight - 3) {
				continue; // if true, ignore the following and go back to the beginning of function
			}
			// if the coordinates are valid, add opponent to the window
			opponent.x = tmpx;
			opponent.y = tmpy;
			opponents.push_back(opponent);
			isFruitEatenByOpponent.push_back(false);

			break;
		}

		opponentsThreads[numberOfOpponents] = std::thread(&fSnakeGame::MoveOpponent,this,numberOfOpponents);
		numberOfOpponents++;
	}


}

void fSnakeGame::DrawOpponent(int x, int y, int oldX,int oldY) {

    this->mutex.lock();
	mvaddch(oldY, oldX, ' ');
	mvaddch(y, x, opponentChar);
	this->mutex.unlock();
    refresh();
	return;

}

bool fSnakeGame::CollisionWithOpponent() {
    for(int j =0; j < snake.size(); j++) {
        for (int i = 0; i < numberOfOpponents; i++) {
            if(snake[j].x ==opponents[i].x && snake[j].y ==opponents[i].y){
            	endGame = true;
                return true;
            }
        }
    }
    return false;

}

bool fSnakeGame::OpponentEatsFruit(int id)
{
    if(opponents[id].x == fruit.x && opponents[id].y == fruit.y) {
		isFruitEatenByOpponent[id] = true;
        //PositionFruit();
        return true;
    }
    return false;
}

void fSnakeGame::Drawing(int x, int y, char drawChar, int oldX, int oldY) {
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	//foreground / backgound
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_YELLOW);

	this->mutex.lock();
    if(oldX !=-1 && oldY!=-1) mvaddch(oldY, oldX, ' ');
	if(drawChar == opponentChar) attron(COLOR_PAIR(3));
	if(drawChar == partchar) attron(COLOR_PAIR(2));
    if(drawChar == fruitchar) attron(COLOR_PAIR(4));
    mvaddch(y, x, drawChar);
    refresh();
	if(drawChar == opponentChar) attroff(COLOR_PAIR(3));
	if(drawChar == partchar) attroff(COLOR_PAIR(2));
    if(drawChar == fruitchar) attroff(COLOR_PAIR(4));
    this->mutex.unlock();
    return;

}

void fSnakeGame::TimeCount() {
	chrono::steady_clock sc;
	auto start = sc.now();
	while(endGame!=true){
		auto end = sc.now();       // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
		auto time_span = static_cast<chrono::duration<double>>(end - start);
		if(time_span.count()>opponentKillTime){
			for(int i = 0 ;i < numberOfOpponents; i++){

				if(isFruitEatenByOpponent[i] == false)KillOpponent(i);
			}
			auto start = sc.now();
		}

	}

}

void fSnakeGame::KillOpponent(int i) {
	move((maxheight-2)/2,(maxwidth-5)/2);
	printw("End Time! KILLED %d", i);
}

void fSnakeGame::EndGame() {

    move((maxheight-2)/2,(maxwidth-5)/2);
    printw("End GAME!");


    //timeThread->join();
    //for(int i=0; i < numberOfOpponents; i++)
    //   opponentsThreads->join();

    //snakeThread->join();
    //nodelay(stdscr, false);
    //getch();
    //endwin();


}

void fSnakeGame::initThreads() {
	snakeThread = new std::thread(&fSnakeGame::PlayGame,this);
	for(int i = 0 ; i< numberOfStartOpponents; i++) {
		AddOpponent();
	}

	refresh();
	//snakeThread = new std::thread(&fSnakeGame::PlayGame,this);
	timeThread = new std::thread(&fSnakeGame::TimeCount,this);

}
