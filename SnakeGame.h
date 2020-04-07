#ifndef _SAKE_GAME_
#define _SAKE_GAME_
#include <iostream>
#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

class SnakeGame
{
private: 
	WINDOW *rootScr {nullptr};					// root screen
	int scrW {}, scrH {};						// Screen width / height
	int xPos {}, yPos {};						// Default coordinates for the snake to start
	int iKey {KEY_RIGHT};						// Default starting direction
	int keyPressed {};							// Pressed key input stored in this
	int points{};								// Points collected after each food
	// It would be awesome to take that from external file
	const char cFood {'o'};							// Char representing food
	const char cWall {'#'};							// Char representing an obstacle
	
	std::vector<std::vector<char>> gameArea;	// Game area will be stored in here, borders and obstacles
	std::vector<std::vector<int>> snake;		// Snake is stored in here.
	std::vector<int> food;						// Coordinates for food location
	std::vector<int> mov;						// Coordinates for movement in checkInput()
	// vector of vectors is not a good idea. it's much faster to use int[][]. vector.at() is reeeeaaaally slow
	std::vector<std::vector<int>>::iterator it;	// Iterator used in checkInput(): adding new head
	
	void terminate(); 														// Terminates the window
	void displayEndScreen();												// Displaying endscreen
	
	void setSnake();														// Starting coordinates for the snake
	void displaySnake();													// Display snake
	
	void setFood();															// Update food location
	void setFood(int x, int y);												// Makes it easier to test
	void consumedFood();													// Check if snake consumed food
	
	void chekInput();														// Check input for new directions
	
	void prepareGameArea();													// Prepares the game area vector (borders and obstacles)
	void displayGameArea();													// Display game area borders and obstacles
	void insertObstacle();													// Insert a wall block in a random place
	
	bool collisionDetection();												// Checking if the snake has collided to anything or itself
	bool ifInSnake(std::vector<std::vector<int>> &v, bool sHead = false);	// Return true if anything is where the snake is
																			// If sHead = true, check whole snake else skip the first element.
	
	
public:
	SnakeGame();
	~SnakeGame();
	
	void play();	// run the game.
};

#endif // _SAKE_GAME_
