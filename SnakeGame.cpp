#include "SnakeGame.h"



SnakeGame::SnakeGame()
{
	initscr();						// Initialize the screen
	curs_set(0);					// Hide cursor
	keypad(stdscr, true);			// Enable special keys, such as KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN
	timeout(100);					// Set the screen update delay / Refresh rate in milliseconds
	getmaxyx(stdscr, scrH, scrW); 	// Get max screen size
	points = 0;
	
	prepareGameArea();				// Prepare game area, this has to be set first.
	setSnake();						// Initialize the snake, this has to  be set before setFood.
	setFood();						// Set the first piece of food
	iKey = KEY_RIGHT;
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
	snake = {{yPos, xPos}, {yPos, xPos - 1}, {yPos, xPos - 2} };
}

/*
 * Display the snake in the game area
 */
void SnakeGame::displaySnake()
{
	// Adds a new head towards the direction of movement
	//mvaddch(snake[0][0], snake[0][1], ACS_CKBOARD);
	
	// add the snake into gameArea instead of an overlay which is just displayed
	gameArea.at(snake[0][0]).at(snake[0][1]) = cSnk;
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
		food = {rand() % (scrH - 3) + 3, rand() % (scrW - 3) + 3};
		if (gameArea[food[0]][food[1]] != cWall || ifInSnake(snake, true) == false)
		{
			// Using overloaded setFood to set the food.
			setFood(food[0], food[1]);
			foodOK = true;
		}
	}
}

void SnakeGame::setFood(int y, int x)
{
	gameArea.at(y).at(x) = cFood;
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
		++points;
	} else {
		// No food consumed, pop out the last element of snake.
		int tailY = snake.at(snake.size() - 1).at(0);
		int tailX = snake.at(snake.size() - 1).at(1);
		snake.pop_back();

		//log("[DEBUG] :: Head Y: "+std::to_string(snake[0][0])+", Head X: "+std::to_string(snake[0][1])+", Tail Y: " +
		//	std::to_string(tailY) + ", Tail X: " + std::to_string(tailX) + ", Length: " + std::to_string(snake.size()));

		mvaddch(tailY, tailX, ' ');			// This is also needed, to clear the snake bits from the end.
		gameArea.at(tailY).at(tailX) = ' '; // Remove the tail bit from the gameArea is also needed.
											// One of these missing, snake grows and grows.. got a fix? please tell me!
	}	
}

// *********************************************************************

/*
 * Check input for new directions
 */
void SnakeGame::chekInput()
{
	// this should be defined elsewhere
	bool bAutopilot = false;
	
	// MOVEMENT
	if (bAutopilot)
	{
		iKey = autopilot(iKey);
	}
	else 
	{
		keyPressed = getch();
		if (keyPressed == KEY_RIGHT || keyPressed == KEY_LEFT || keyPressed == KEY_DOWN || keyPressed == KEY_UP)
			iKey = keyPressed;
		else 
		{
			iKey = iKey;
		}
	}
	
	mov = {snake[0][0], snake[0][1]}; // movement {Y,X}
	
	switch (iKey)
	{
		case KEY_RIGHT:
			++mov[1];
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

int SnakeGame::autopilot(int k)
{
	getch();	// keyboard input is needed to run things around
				// even if we dont use it.. any other ways to do it, let me know..
	
	int curX = snake[0][1];
	int curY = snake[0][0];
	
	return hValue(curY,curX,food[0],food[1]);
}

// *********************************************************************

/*
 * Close ncurses screen
 */
void SnakeGame::terminate()
{
	delwin(stdscr);
	endwin();

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
	 mvaddstr(0, 0, text.c_str());
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
			if (i == sHeight - 1 || j == sWidth - 1 || i == 0 || j == 0)
			{
				gameArea.at(i).at(j) = cWall;
			}
			else 
			{
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
			if (gameArea.at(i).at(j) == cWall || gameArea.at(i).at(j) == cFood || gameArea.at(i).at(j) == cSnk)
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
		logEvent("[INFO] ::  Hit the wall!");
		return true;
	} 
	else 
	{
		// Check if snake bit itself
		if (ifInSnake(snake))
		{
			logEvent("[INFO] :: Snake hit itself!");
			return true;
		}
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
		if (gameArea[obstacle[0]][obstacle[1]] != cWall || ifInSnake(snake, true) == false)
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
		displaySnake();				// Display snake ( has to be set before displaying the gameArea )
		displayGameArea();			// Displaying game area borders and obstacles
									// This has to be in the main loop, to update 
									// food and obstacle locations.

		chekInput();				// Check input for new directions
		consumedFood();				// Check if the food was consumed
	}
	
	displayEndScreen();				// Display score
	terminate();					// Terminate.
}


/*
 * Calculating Heuristic value from position to end.
 * Manhattan style calculation, where are only 4 possible directions to go.
 * input current position (Y,X) and the end position (Y,X),
 * function will calculate values from X+1, X-1, Y+1, Y-1
 * and will return the next direction according to smallest value.
 */
int SnakeGame::hValue(int posY, int posX, int fPosY, int fPosX)
{
	// calculations for 4 possible directions (up, down, left, right)
	static int previousValue {};
	int newKey {};
	int safeGuard {0};
	bool selectionOK {false};
	bool upOK {true};
	bool downOK {true};
	bool leftOK {true};
	bool rightOK {true};
	
	// Up
	int upValue {abs(posX - fPosX) + abs((posY - 1) - fPosY)};
	
	// Down
	int downValue {abs(posX - fPosX) + abs((posY + 1) - fPosY)};
	
	// Left
	int leftValue {abs((posX - 1) - fPosX) + abs(posY - fPosY)};
	
	// Right
	int rightValue {abs((posX + 1) - fPosX) + abs(posY - fPosY)};
	
	// DEBUGGING
	//timeout(-1);
	std::string strUp {"UP: " + std::to_string(upValue)};
	std::string strDown {"Down: " + std::to_string(downValue)};
	std::string strLeft {"Left: " + std::to_string(leftValue)};
	std::string strRight {"Right: " + std::to_string(rightValue)};
	std::string curPos {"Current (Y, X): " + std::to_string(posY) + ", " + std::to_string(posX)};
	
	logEvent("[DEBUG] :: " + curPos + ", Food position (Y, X): "+ std::to_string(fPosY) + ", " + std::to_string(fPosX));
	logEvent("[DEBUG] :: " + strUp + ", " + strDown + ", " + strLeft + ", " + strRight);
	logEvent("[DEBUG] :: PreviousKey: " + std::to_string(previousValue));
	
	while (!selectionOK)
	{
		if (posY != fPosY || posX != fPosX)
		{
			if (upValue <= downValue && upValue <= leftValue && upValue <= rightValue && upOK)
			{
				if (predictSuccessfulMovement(KEY_UP,posY,posX))
				{
					logEvent("[Prediction] :: Collision ahead if moving UP, diverting!");
					upOK = false;
					/*if (leftValue <= rightValue)
					{
						Logger::log("[Prediction] :: LEFT");
						previousValue = KEY_LEFT;
						newKey = KEY_LEFT;
					}
					else
					{
						Logger::log("[Prediction] :: RIGHT");
						previousValue = KEY_RIGHT;
						newKey = KEY_RIGHT;
					}*/
				}
				else
				{
					logEvent("[Movement] :: UP");
					previousValue = KEY_UP;
					newKey = KEY_UP;
					selectionOK = true;
				}
					
			}
				
			else if (downValue <= upValue && downValue <= leftValue && downValue <= rightValue && downOK)
			{
				if (predictSuccessfulMovement(KEY_DOWN,posY,posX))
				{
					logEvent("[Prediction] :: Collision ahead if moving DOWN, diverting!");
					downOK = false;
					/*if (leftValue <= rightValue)
					{
						Logger::log("[Prediction] :: LEFT");
						previousValue = KEY_LEFT;
						newKey = KEY_LEFT;
					}
					else
					{
						Logger::log("[Prediction] :: RIGHT");
						previousValue = KEY_RIGHT;
						newKey = KEY_RIGHT;
					}*/
				}
				else
				{
					logEvent("[Movement] :: DOWN");
					previousValue = KEY_DOWN;
					newKey = KEY_DOWN;
					selectionOK = true;
				}
			}
				
			else if (leftValue <= rightValue && leftValue <= upValue && leftValue <= downValue && leftOK)
			{
				if (predictSuccessfulMovement(KEY_LEFT,posY,posX))
				{
					logEvent("[Prediction] :: Collision ahead if moving LEFT, diverting!");
					leftOK = false;
					/*
					if (upValue <= downValue)
					{
						Logger::log("[Prediction] :: UP");
						previousValue = KEY_UP;
						newKey = KEY_UP;
					}
					else
					{
						Logger::log("[Prediction] :: DOWN");
						previousValue = KEY_DOWN;
						newKey = KEY_DOWN;
					}*/
				}
				else
				{
					logEvent("[Movement] :: LEFT");
					previousValue = KEY_LEFT;
					newKey = KEY_LEFT;
					selectionOK = true;
				}
			}
				
			else if (rightValue <= leftValue && rightValue <= upValue && rightValue <= downValue && rightOK)
			{
				if (predictSuccessfulMovement(KEY_RIGHT,posY,posX))
				{
					logEvent("[Prediction] :: Collision ahead if moving RIGHT, diverting!");
					rightOK = false;
					/*
					if (upValue <= downValue)
					{
						Logger::log("[Prediction] :: UP");
						previousValue = KEY_UP;
						newKey = KEY_UP;
					}
					else
					{
						Logger::log("[Prediction] :: DOWN");
						previousValue = KEY_DOWN;
						newKey = KEY_DOWN;
					}*/
				}
				else
				{
					logEvent("[Movement] :: RIGHT");
					previousValue = KEY_RIGHT;
					newKey = KEY_RIGHT;
					selectionOK = true;
				}
			}
			else
			{
				if (predictSuccessfulMovement(KEY_UP,posY,posX))
					logEvent("[Prediction] :: Collision ahead if moving previous direction! SHIT HAPPENS!");
					
				logEvent("[Fallback] :: next movement will be previous: " + std::to_string(previousValue));
				newKey = previousValue;
				selectionOK = true;
			}
		} 
		else 
		{
				if (predictSuccessfulMovement(KEY_UP,posY,posX))
					logEvent("[Prediction] :: Collision ahead if moving previous direction! oh god, why?");
					
			logEvent("[Fallback] :: Snake pos Y/X matches to Food Y/X");
			newKey = previousValue;
			selectionOK = true;
		}
		
		++safeGuard;
		if (safeGuard == 4)
			break;
	}
	
	return newKey;
}


// A simple logging mechanism to help debugging
/*bool SnakeGame::log(std::string str)
{
	if (!bLogging)
		return false;
		
	std::ofstream logFile {"log.txt", std::ios_base::app};
	if (logFile.is_open())
	{
		logFile << str << std::endl;
		logFile.close();
		return true;
	}
	else return false;
}*/

// direction is the new heading nad y/x pos is the current position of the snake head.
bool SnakeGame::predictSuccessfulMovement(int direction, int yPos, int xPos)
{
	bool bResult {false};
	//258 down, 259 up, 260 left, 261 right
	switch (direction)
	{
		case 259: 	// UP
		{
			if (cWall == gameArea[yPos-1][xPos] || cSnk == gameArea[yPos-1][xPos])
				bResult =  true;
			break;
		}
			
		case 258:	// DOWN
		{
			if (cWall == gameArea[yPos+1][xPos] || cSnk == gameArea[yPos+1][xPos])
				bResult = true;
			break;
		}
			
		case 260:	// LEFT
		{
			if (cWall == gameArea[yPos][xPos-1] || cSnk == gameArea[yPos][xPos-1])
				bResult = true;
			break;
		}
			
		case 261:	// RIGHT
		{
			if (cWall == gameArea[yPos][xPos+1] || cSnk == gameArea[yPos][xPos+1])
				bResult = true;
			break;
		}
	}
	
	return bResult;
}