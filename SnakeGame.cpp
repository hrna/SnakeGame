#include "SnakeGame.h"


SnakeGame::SnakeGame()
{
	initscr();	// Initialize the screen
	curs_set(0);					// Hide cursor
	keypad(rootScr, true);			// Enable special keys, such as KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN
	timeout(100);					// Set the screen update delay / Refresh rate in milliseconds
	getmaxyx(rootScr, scrH, scrW); 	// Get max screen size
	points = 0;
	
	prepareGameArea();				// Prepare game area, this has to be set first.
	setSnake();						// Initialize the snake, this has to  be set before setFood.
	setFood();						// Set the first piece of food
}

SnakeGame::~SnakeGame()
{
	terminate();
}

// *********************************************************************

/*
 * Initializing the snake 
 */
void SnakeGame::setSnake()
{
	// Default starting position for the snake
	xPos = scrW / 4;
	yPos = scrH / 2;
	
	// Initialize vectors holding the snake X & Y coordinates
	snake = {{yPos, xPos}, {yPos,xPos - 1}, {yPos, xPos - 2} };
}

/*
 * Display the snake in the game area
 */
void SnakeGame::displaySnake()
{
	// Adds a new head towards the direction of movement
	mvaddch(snake[0][0], snake[0][1], ACS_CKBOARD);
}

// *******************************************************************

/*
 * Set a piece of food to a random position and display it.
 * The food is inserted in gamearea vector and checked that it cannot be inside walls or in snake
 */
void SnakeGame::setFood()
{
	srand(time(nullptr));
	bool foodOK {false};
	
	while (foodOK == false)
	{
		food = {rand() % (scrH-3) + 3, rand() % (scrW-3) + 3};
		if (gameArea[food[0]][food[1]] != cWall || ifInSnake(snake, true) == false)	// not important, I just like space after comma :D
		{
			setFood(food[0], food[1]);
			foodOK = true;
		}
	}
}

// This way you'll be able to write tests which will place food in the place you need it
// for example to check does the snake eats food properly
void SnakeGame::setFood(int x, int y)
{
	gameArea.at(food[0]).at(food[1]) = cFood;
}

/*
 * Check if the snake consumed the food
 */
void SnakeGame::consumedFood()
{
	if (snake[0][0] == food[0] && snake[0][1] == food[1])
	{
		// Snake consumed food, remove old food and set new food.
		// Add an obstacle
		gameArea.at(food[0]).at(food[1]) = ' ';
		insertObstacle();
		setFood();
		++points;	// ++i is faster than i++. the second one creates a copy, increments value and assignes it back.
		
	} else {
		// No food consumed, pop out the last element of snake.
		mvaddch(snake.at(snake.size() - 1).at(0), snake.at(snake.size() - 1).at(1), ' ');
		snake.pop_back();
	}	
}

// *********************************************************************

/*
 * Check input for new directions
 */
void SnakeGame::chekInput()
{
	// MOVEMENT
	keyPressed = getch();
	if (keyPressed == KEY_RIGHT || keyPressed == KEY_LEFT || keyPressed == KEY_DOWN || keyPressed == KEY_UP)
		iKey = keyPressed;
	else
	{
		iKey = iKey;
	}
	
	mov = {snake[0][0], snake[0][1]}; // movement {Y,X}
	
	switch (iKey)
	{
		case KEY_RIGHT:
			++mov[1];	// not important, I just like it this way :D
			break;
		case KEY_LEFT:
			--mov[1];
			break;
		case KEY_UP:
			--mov[0];
			break;
		case KEY_DOWN:
			++mov[0];
			break;
	}	
	it = snake.begin();
	snake.insert(it, mov);
}

// *********************************************************************

/*
 * Close ncurses screen
 */
void SnakeGame::terminate()
{
	delwin(rootScr);
	endwin();
	// what aboud deleting rootScr? is it needed? 
}

/*
 * Displaying endscreen with points.
 */
 void SnakeGame::displayEndScreen()
 {
	 clear();
	 timeout(-1);
	 std::string text {"Game over, points received: " + std::to_string(points) + 
						", congratulations and keep up the good spirit!\n"};
	 mvaddstr(0, 0, text.c_str());	// again, I like 
	 mvaddstr(1, 0, "Press any key to quit");
	 getch(); // wait for any key pressed 
 }

// *********************************************************************

/*
 * Prepares the game area vector
 * borders and obstacles will be painted as defined in cWall variable
 */
void SnakeGame::prepareGameArea()
{
	size_t sHeight {static_cast<size_t>(scrH)};
	size_t sWidth {static_cast<size_t>(scrW)};
	
	gameArea.resize(sHeight, std::vector<char>(sWidth));
	
	for (size_t i{0}; i < sHeight; i++)
		for (size_t j{0}; j < sWidth; j++)
		{
			// 1. it's easier to see "words" if you'll enter spaces around minus
			// 2. it's easy to misspell "==" and write "=". if you'll compare this way 0 == i and if you'll make a typo, compiler will tell you that it's wrong
			if (i == sHeight - 1 || j == sWidth - 1 || 0 == i || 0 == j)
			{
				gameArea.at(i).at(j) = cWall;
			}
			else {
				gameArea.at(i).at(j) = ' ';
			}
		}
}

/*
 * Display game area borders and obstacles
 * 2D vector borders Y , X
 * Displaying only if matching to wall or an obstacle
 */
void SnakeGame::displayGameArea()
{
	for (size_t i{0}; i < gameArea.size(); i++)
		for (size_t j{0}; j < gameArea.at(i).size(); j++)
		{
			if (gameArea.at(i).at(j) == cWall || gameArea.at(i).at(j) == cFood)
			mvaddch(i, j, gameArea.at(i).at(j));
		}
}

/*
 * Checking if the snake has collided to anything or itself
 * Returns boolean value, which helps to break out the loop
 */
bool SnakeGame::collisionDetection()
{
	
	int y = snake[0][0];
	int x = snake[0][1];
	
	if (cWall == gameArea[y][x])
	{
		return true;
	}
	else
	{
		// Check if snake bit itself
		if (ifInSnake(snake))
			return true;
	}
	
	return false;
}

/*
 * Check if anything is in the snake (player, or later maybe cpu)
 * if sHead = true, check through whole snake
 * else if sHead = false, skip the head (the first element)
 */
bool SnakeGame::ifInSnake(std::vector<std::vector<int>> &v, bool sHead)
{
	int y = v[0][0];
	int x = v[0][1];
	
	if (sHead)
	{
		// Check through whole lenght
		for (size_t i{0}; i < v.size();i++)
		{
			if (y == v.at(i).at(0) && x == v.at(i).at(1))
				return true;
		}		
	} else {
		// Skip the head bit
		for (size_t i{1}; i < v.size();i++)
		{
			if (y == v.at(i).at(0) && x == v.at(i).at(1))
				return true;
		}
	}
	
	return false;
}

/*
 * Insert a random blocking object to game area
 */
void SnakeGame::insertObstacle()
{
	srand(time(nullptr));
	bool obstacleOK {false};
	std::vector<int> obstacle {0, 2};
	
	while (obstacleOK == false)
	{
		obstacle = {rand() % (scrH - 2) + 2, rand() % (scrW - 2) + 2};
		if (gameArea[obstacle[0]][obstacle[1]] != cWall || false == ifInSnake(snake,true))
		{
			// A single block elment
			gameArea.at(obstacle[0]).at(obstacle[1]) = cWall;
			
			// additional 3 to create 2x2 block
			gameArea.at(obstacle[0] + 1).at(obstacle[1]) = cWall;
			gameArea.at(obstacle[0]).at(obstacle[1] + 1) = cWall;
			gameArea.at(obstacle[0] + 1).at(obstacle[1] + 1) = cWall;
			obstacleOK = true;
		}
	}	
}

void SnakeGame::play()
{
	while (!collisionDetection())	// Run the game unless snake hit the wall or itself
	{
		displayGameArea();			// Displaying game area borders and obstacles
									// This has to be in the main loop, to update 
									// food and obstacle locations.

		chekInput();				// Check input for new directions
		consumedFood();				// Check if the food was consumed
		displaySnake();				// Display snake
	}
	
	displayEndScreen();				// Display score
	terminate();					// Terminate.
}