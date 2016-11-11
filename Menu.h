#include <stdio.h>
#include <ncurses.h>

#define WIDTH 30
#define HEIGHT 10 
#define ENTER 10

int startx = 0;
int starty = 0;

char *choices[] = { 
			"Play Game",
			/*"Set Bodies",*/
			"Exit",
		  	};
int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *menu_win, int highlight);
int game_menu();

int game_menu()
{	
	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;
	int X_MAX, Y_MAX;

	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	
	getmaxyx(stdscr, Y_MAX, X_MAX);
	
	startx = (X_MAX - WIDTH) / 2;
	starty = (Y_MAX - HEIGHT) / 2;
		
	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);
	print_menu(menu_win, highlight);
	while(1)
	{	c = wgetch(menu_win);
		switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = n_choices;
				else
					--highlight;
				clear();
				refresh();
				break;
			case KEY_DOWN:
				if(highlight == n_choices)
					highlight = 1;
				else 
					++highlight;
				clear();
				refresh();
				break;
			case ENTER:
				choice = highlight;
				clear();
				refresh();
				break;
			default:
				mvprintw(Y_MAX/2 + n_choices + 3, X_MAX/2 - 21, "Press UP, DOWN to choose & ENTER to select");
				refresh();
				break;
		}
		print_menu(menu_win, highlight);
		if(choice != 0)	/* User did a choice come out of the infinite loop */
			break;
	}
	clrtoeol();
	refresh();
	endwin();
	return choice;
}


void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;	

	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}
