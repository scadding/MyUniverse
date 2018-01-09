//******************************************************************************
//	Include Files
//******************************************************************************
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ncurses.h>

#include <unistd.h>

#include <string.h>
#include <ctype.h>

#include "screen.h"

//******************************************************************************
//	Static Data
//******************************************************************************
static char * txtContinue = "<press any key to continue>";

//******************************************************************************
//	Beep at the user.
//******************************************************************************
void cScreen::Beep(void)
{
/*	sound(1000);
	delay(200);
	nosound();*/
	beep();
}
//******************************************************************************
//	Misc inlines
//******************************************************************************
void cScreen::Clear(void) {
	clear();
}

void cScreen::Go(int Row) {
	move(Row, 1);
	refresh();
}

void cScreen::Go(int Row, int Col) {
	move(Row, Col);
	refresh();
}

void cScreen::CursorOn(void) {
}

void cScreen::CursorOff(void) {
}

//******************************************************************************
//	Print out an error message. 
//******************************************************************************
void cScreen::ErrMsg(char * Msg) {
    int Col;

    init_pair(C_YELLOW_RED, COLOR_WHITE, COLOR_RED);

    fflush(stdin);
    Col = CalcCTextCol(Msg);
    wmove(wstat, 0, Col);
    wattrset(wstat, COLOR_PAIR(C_YELLOW_RED));
    wrefresh(wstat);
    wprintw(wstat,"%s", Msg);
    wrefresh(wstat);

    Col = CalcCTextCol( txtContinue);
    wmove(wstat, 1, Col);
    wrefresh(wstat);
    wprintw(wstat,txtContinue);
    wrefresh(wstat);

    Beep();
    if (getchar() == 0)
    	getchar();

}
//******************************************************************************
//	Display a list of items and let the user pick one.
//******************************************************************************
int cScreen::ListBox(int ItemCnt, ...)
{
    int Row;
    int Col;
    int CurRow;
    int i;
    char * pItem;
    va_list pArgs;

    clear();

    CurRow = Row = CalcCTextRow(ItemCnt);
    va_start(pArgs,ItemCnt);

    // Determine the starting column based on the width of the items.
    // This assumes all items are the same width (blank padded).
    pItem = va_arg(pArgs, char *);
    Col = CalcCTextCol(pItem);

    // Print out the list box items.
    for (i = 0; i < ItemCnt; i++) {
    	wmove(wmain, CurRow++, Col);
	wrefresh(wmain);
	wprintw(wmain, "%c) %s",i + 'a',pItem);
	pItem = va_arg(	pArgs, char *);
    }
    CurRow = Row;
    wmove(wmain, CurRow, Col);
    wrefresh(wmain);

    CursorOn();

    // Get the choice.
    do {
    	switch(i = getchar()) {
	    case 0:    	    
		CurRow = ProcessArrowKey(Row,CurRow,ItemCnt,Col);
		i = -1;
		break;
    	    case KEY_UP: /* up */
		if (CurRow != Row) {
    		    CurRow--;
		    wmove(wmain, CurRow, Col);
		    wrefresh(wmain);
		}
		else
		    Beep();
		i = -1;
		break;
	    case KEY_DOWN: /* down */
		if ((CurRow - Row + 1) < ItemCnt) {
		    CurRow++;
		    wmove(wmain, CurRow, Col);
		    wrefresh(wmain);
		}
		else
		    Beep();
		i = -1;
		break;
	    case ' ':
	    case '\r':
	    case '\n':
		i = CurRow - Row;
		break;
	    default:
		i = tolower(i) - 'a';
		if (i < 0 || i >= ItemCnt)
		    Beep();
		}
	} while ( i < 0 || i >= ItemCnt);

    va_end(pArgs);
    return(i);
}
//******************************************************************************
//	Calculate the starting row to use for centering text vertically.
//******************************************************************************
int cScreen::CalcCTextRow(int NumLines) {
	int Row;
	Row = (LINES - NumLines) / 2;
	if (Row < 1)
		Row = 1;
	return(Row);
}
//******************************************************************************
//	Calculate the starting col to use for centering text horizontally.
//******************************************************************************
int cScreen::CalcCTextCol( char * Text) {
	int Col;
	Col = (COLS - strlen(Text)) / 2;
	if (Col < 1)
		Col = 1;
	return(Col);
}
//******************************************************************************
//	Move the current cursor position up or down based on an arrow key.
//******************************************************************************
int cScreen::ProcessArrowKey(int FirstRow,int CurRow,int NumRows,int Col) {
	switch(getchar()) {
		case KEY_UP:
			if (CurRow != FirstRow) {
				CurRow--;
				Go(CurRow, Col);
			}
			else
				Beep();
			break;
		case KEY_DOWN:
			if ((CurRow - FirstRow + 1) < NumRows) {
				CurRow++;
				Go(CurRow, Col);
			}
			else
				Beep();
			break;
		default:
			Beep();
	}
	return(CurRow);
}
