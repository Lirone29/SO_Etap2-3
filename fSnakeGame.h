/*
This is the game logic (no user interaction).
The game is a very simple version of the classic snake game.
*/

#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>

#ifndef FSNAKEGAME_H
#define FSNAKEGAME_H

// Unreal standards
using int32 = int;
using namespace std;

struct CharPosition 
{
	int32 x, y;
	CharPosition(int32 col, int32 row);
	CharPosition();
};

class fSnakeGame 
{
private:
    int opponentWaitTime, opponentKillTime;
	int numberOfOpponents, numberOfStartOpponents, maxNumberOfOpponents;
	int32 score, del, maxwidth, maxheight;
	char direction, partchar, edgechar, fruitchar, opponentChar;

	// partchar is the character representing the snake's body
	// edgechar is the character representing the edge of the game window
	// fruitchar is the character representing the fruit
	// opponentChar is the character representing the opponent
	// del stands for delay

	bool bEatsFruit, endGame;
	CharPosition fruit; // need to clarify this combination 
	std::vector<CharPosition> snake; // represent the snake's body
	std::vector<CharPosition> opponents;
	std::thread* timeThread;
	std::thread* snakeThread;
	std::thread* opponentsThreads;
	//std::vector<std::thread> threads;
	std::vector<bool> isFruitEatenByOpponent;
	mutex mutex;

	void InitGameWindow();
	void DrawWindow();  
	void DrawSnake();
	void PrintScore();
	void PositionFruit();
	bool FatalCollision();
	void MoveSnake();
	bool GetsFruit();

	void KillOpponent(int i);
	void AddOpponent();
	void DrawOpponent(int x, int y, int oldX, int oldY);
	void MoveOpponent(int id);
	bool CollisionWithOpponent();
	bool OpponentEatsFruit(int id);
	void TimeCount();
	void initThreads();
public:
    void Drawing(int x, int y, char drawChar, int oldX, int oldY);
	fSnakeGame();
	~fSnakeGame(); // destructor for cleanup and memory deallocation
	void PlayGame();
	void EndGame();
};

#endif