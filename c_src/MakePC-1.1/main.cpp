#include <stdlib.h>
#include <stdio.h>

#include "dice.h"
#include "menu.h"
#include "screen.h"

cDice	Dice;
cScreen	Screen;

void finish(int);

WINDOW *wmain;
WINDOW *wstat;
WINDOW *wmenu;

//**********************************************************************
//	Main
//**********************************************************************
int main() {
    cMenu Menu;
    initscr();
    if (has_colors()) {
	start_color();
    }

    savetty();
    cbreak();
    wmain = newwin(LINES-3, 0, 0, 0);
    wstat = newwin(0, 0, LINES-2, 0);
    wmenu = newwin(0, 0, LINES-3, 0);
    
    Menu.ProcessCmds();

    finish(0);
}

void finish(int sigage)
{
    curs_set(1);
    clear();
    refresh();
    resetty();
    endwin();
    exit(0);
}
