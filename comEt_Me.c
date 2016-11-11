#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<limits.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

#include "Menu.h"

#define SPACESHIP -1
#define RADIUS 15
#define BODIES 3
#define MASS 300
#define G 0.15
#define FULL_TANK 15
#define THRUST 0.5
#define TIMESTEP 5
#define GAME_ON 0
#define WON 1
#define LOST -1


float x, y, X_MAX, Y_MAX, X_MIN, Y_MIN, **body, fuel, Exit;	//x,y = position of spacecraft, *_MAX, *_MIN = boundary of the arena
float x_vel, y_vel, last_x, last_y;
int state;	// 0 = Game On, 1 = Win, -1 = Lose

WINDOW* init();	// Initialize the game variables
int step();	// Time Step
int moveShip(int key); // Move the SpaceShip if needed
int drawChar(WINDOW *game_win, float y, float x, char *ch); // Draw each character on screen
int draw(WINDOW *game_win); // Paint the canvas
int gameCheck(); // Check if the player has won the game, or lost
int playGame(); // Play

WINDOW *game_win;

WINDOW *init()
{
	state = GAME_ON; // Game state set as RUNNING
	
	X_MIN = 0;
	Y_MIN = 0;
	
	initscr();
	noecho();
	cbreak();
	clear();
	
	getmaxyx(stdscr, Y_MAX, X_MAX);
	
	game_win = newwin(Y_MAX - Y_MIN - 2, X_MAX - X_MIN - 2, Y_MIN + 1, X_MIN + 1);
	keypad(game_win, TRUE);
	
	X_MAX -= 3;
	//X_MIN++;
	Y_MAX -= 3;
	//Y_MIN++;
	
	x = X_MIN + 1;
	y = (random()/(float)RAND_MAX * Y_MAX) + Y_MIN;
	x_vel = 0.0f;
	y_vel = 0.0f;
	fuel = FULL_TANK;
	
	last_x = x;
	last_y = y;
	
	// Bodies defined below
	
	body = calloc(BODIES, sizeof(float));
	int i = BODIES + 1;
	while(i-- != 0)
	{
		body[i] = calloc(5, sizeof(float));
		body[i][0] = (random()/(float)RAND_MAX * X_MAX) + X_MIN;	// X coordinates of the body
		body[i][1] = (random()/(float)RAND_MAX * Y_MAX) + Y_MIN;	// Y coordinates of the body
		body[i][2] = random()/(float)RAND_MAX * MASS;			// Mass of the body
		body[i][3] = (body[i][2]/(float)MASS * RADIUS);			// Radius of the body
		
	}
	Exit = (random()/(float)RAND_MAX * Y_MAX) + Y_MIN;		// Y Coordinates of Exit
	return game_win;
}
int step()
{
	int i = BODIES;
	while(i-- != 0)
	{
		float x_b = body[i][0], y_b = body[i][1], a = G * body[i][2] / pow((x_b - x)*(x_b - x) + (y_b - y)*(y_b - y), 1.5);
		x_vel += a * (x_b - x);
		y_vel += a * (y_b - y);
	}
	
	last_x = x;
	last_y = y;
	
	x += x_vel;
	y += y_vel;
}
int moveShip(int key)
{
	switch(key)
	{
		case KEY_UP:
			y--;
			y_vel -= THRUST;
			fuel--;
			return 0;
		case KEY_DOWN:
			y++;
			y_vel += THRUST;
			fuel--;
			return 0;
		case KEY_RIGHT:
			x++;
			x_vel += THRUST;
			fuel--;
			return 0;
		case KEY_LEFT:
			x--;
			x_vel -= THRUST;
			fuel--;
			return 0;
		default:
			return 1;
	}
}
int drawChar(WINDOW *game_win, float yf, float xf, char* ch)
{
	int x = (int)(xf + 0.5), y = (int)(yf + 0.5);
	if(x >= X_MAX || x <= X_MIN || y >= Y_MAX || y <= Y_MIN)
		return 1;
	mvwprintw(game_win, y, x, "%s", ch);
	//wrefresh(game_win);
}
int draw(WINDOW *game_win)
{
	wclear(game_win);
	box(game_win, 0, 0);
	wrefresh(game_win);
	
	/*
	 * Spaceship Graphical representation:
	 *
	 *  ,,, 
	 * :|S|>
	 *  ''' 
	 *
	 */
	 
	drawChar(game_win, y + 1, x - 1, ",");
	drawChar(game_win, y + 1, x, ",");
	drawChar(game_win, y + 1, x + 1, ",");
	drawChar(game_win, y, x - 2, ":");
	drawChar(game_win, y, x - 1, "|");
	drawChar(game_win, y, x, "S");
	drawChar(game_win, y, x + 1, "|");
	drawChar(game_win, y, x + 2, ">");
	drawChar(game_win, y - 1, x - 1, "\'");
	drawChar(game_win, y - 1, x, "\'");
	drawChar(game_win, y - 1, x + 1, "\'");
	wrefresh(game_win);
	
	/*
	 * 
	 * Bodies' Representation
	 * 
	 * %
	 * 
	 */
	
	int i = BODIES;
	while(i-- != 0)
	{
		mvwprintw(game_win, body[i][1], body[i][0], "%d", body[i][2]);
		float a_x = body[i][0] - body[i][3], b_x = body[i][0] + body[i][3], a_y = body[i][1] - body[i][3], j = a_x, k, end;
		while(j < b_x)
		{
			k = body[i][1] - sqrt(body[i][3] * body[i][3] - (j - body[i][0]) * (j - body[i][0]));
			end = body[i][1] + sqrt(body[i][3] * body[i][3] - (j - body[i][0]) * (j - body[i][0]));
			
			while(k < end)
			{
				drawChar(game_win, k, j, "%");
				k++;
			}
			j++;
		}
		
		wattron(game_win, A_REVERSE);
		
		mvwprintw(game_win, (int)(body[i][1] - 1), (int)(body[i][0] - 3), "     ", body[i][2]);
		mvwprintw(game_win, (int)(body[i][1]), (int)(body[i][0] - 3), " %3.0f ", body[i][2]);
		float acc = G * body[i][2] / pow((body[i][0] - x)*(body[i][0] - x) + (body[i][1] - y)*(body[i][1] - y), 0.5);
		mvwprintw(game_win, (int)(body[i][1] + 1), (int)(body[i][0] - 3), " %3.0f ", acc);
		
		wattroff(game_win, A_REVERSE);
		
	}
	mvwprintw(game_win, (int)(Exit - 1), X_MAX, "/");
	mvwprintw(game_win, (int)Exit, X_MAX, "{");
	mvwprintw(game_win, (int)(Exit + 1), X_MAX, "\\");
	
	wattron(game_win, A_REVERSE);
	
	mvwprintw(game_win, (int)(Y_MAX - 5), (int)(0.5 * (X_MAX + X_MIN) - 9), "X-Velocity = %+2.3f" , x_vel);
	mvwprintw(game_win, (int)(Y_MAX - 4), (int)(0.5 * (X_MAX + X_MIN) - 9), "Y-Velocity = %+2.3f" , -y_vel);
	mvwprintw(game_win, (int)(Y_MAX - 3), (int)(0.5 * (X_MAX + X_MIN) - 9), " Fuel Left =  %3.0f  ", fuel);
	
	wattroff(game_win, A_REVERSE);
	
	/* ** Uncomment to see edges **
	mvwprintw(game_win, Y_MIN, X_MIN, "xy");
	mvwprintw(game_win, Y_MIN, X_MAX, "Xy");
	mvwprintw(game_win, Y_MAX, X_MAX, "XY");
	mvwprintw(game_win, Y_MAX, X_MIN, "xY");
	*/
	
	wrefresh(game_win);
	return 0;
}
int gameCheck()
{
	if(fuel == 0)	// Fuel Finished
	{
		state = LOST;
		return 0;
	}
	if(x == X_MAX && y > Exit - 1 && y < Exit + 1) // Perfect Landing
	{
		state = WON;
		return 0;
	}
	if(last_x == x)
		return 1;
	else if(x > X_MAX &&  (X_MAX - last_x) * (y - last_y) == (Exit + 1 - last_y) * (x - last_x))
	{
		state = WON;
		return 0;
	}
	else if(x > X_MAX && (int)((X_MAX - last_x) * (y - last_y)) == (int)((Exit - last_y) * (x - last_x)))
	{
		state = WON;
		return 0;
	}
	else if(x > X_MAX && (X_MAX - last_x) * (y - last_y) == (Exit - 1 - last_y) * (x - last_x))
	{
		state = WON;
		return 0;
	}
	else
	{
		int i = BODIES;
		while(i-- != 0)
		{
			float xb = body[i][0], yb = body[i][1], rb = body[i][3], r = sqrt((yb - y)*(yb - y) + (xb - x)*(xb - x));
			if(r <= rb)
			{
				state = LOST;
				return 0;
			}
		}
		return 1;
	
	}
}
int print_win(WINDOW *game_win)
{
	wclear(game_win);
	box(game_win, 0, 0);
	int x = (int)(0.5 * (X_MAX + X_MIN)), y = (int)(0.5 * (Y_MAX + Y_MIN));
	char *message = "Congratulations! You Won!!!";
	int l = strlen(message);
	x -= l/2;
	mvwprintw(game_win, y, x, message);
	wrefresh(game_win);
	return 0;
}
int print_lost(WINDOW *game_win)
{
	wclear(game_win);
	box(game_win, 0, 0);
	int x = (int)(0.5 * (X_MAX + X_MIN)), y = (int)(0.5 * (Y_MAX + Y_MIN));
	char *message = "Sorry! You've Lost!!!";
	int l = strlen(message);
	x -= l/2;
	mvwprintw(game_win, y, x, "%s", message);
	wrefresh(game_win);
	return 0;
}
int playGame()
{
	WINDOW *game_win = init();
	srand( time(NULL));
	halfdelay(TIMESTEP);
	while(gameCheck())
	{
		int ch;
		while(ERR == (ch = wgetch(game_win)) && gameCheck())
		{
			draw(game_win);
			step();
		}
		moveShip(ch);
	}
	if(state == WON)
		print_win(game_win);
	else if(state == LOST)
		print_lost(game_win);
	wtimeout(game_win, -1);
	while(ERR == wgetch(game_win)){};
	//pause();
	clrtoeol();
	refresh();
	endwin();
	return 0;
}
int main()
{
	int play = 1;
	while(play != 2)
	{
		play = game_menu();
		if(play == 1)
			playGame();
	}
}
