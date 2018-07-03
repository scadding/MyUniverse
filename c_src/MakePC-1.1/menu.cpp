//**********************************************************************
//	Include Files
//**********************************************************************
#include <stdio.h>
#include <ncurses.h>

#include <ctype.h>
#include <string.h>

#include "menu.h"
#include "screen.h"
#include "dice.h"

//**********************************************************************
//	Static Data
//**********************************************************************
static char * txtCmds =	"Quit  Race  Stats  Age  Background  File";
static char * txtFileName = "File name: ";
static char * errNoRace = "Sorry, you must select a 'Race' first.";
static char * txtHasStats = "Do you have pre-rolled stats (y/n) ? ";
static char * errBadValue = "Sorry, that is an invalid number.";
static char * txtPCAge = "What is the characters age (min=15): ";
static char * txtAccept = "Accept (y/n) ? ";

//**********************************************************************
//	Constructor
//**********************************************************************
cMenu::cMenu(void) {
	PC = NULL;
	Backgrnd = NULL;
}

//**********************************************************************
//	Process the user commands.
//**********************************************************************
void cMenu::ProcessCmds(void) {
    int	Col;
    char Cmd;

    init_pair(C_WHITE_BLUE, COLOR_WHITE, COLOR_BLUE);

    while (1) {
	wclear(wmain);
	if (PC != NULL)
		PC->Output();
	if (Backgrnd != NULL)
		Backgrnd->Output(*PC);
	Col = Screen.CalcCTextCol(txtCmds);
	wmove(wmenu, 0, Col);
	wrefresh(wmenu);
        wattrset(wmenu, COLOR_PAIR(C_WHITE_BLUE));
	wprintw(wmenu, txtCmds);
	Screen.CursorOff();
	wrefresh(wmenu);
	wmove(wmenu, 0, Col);
	wrefresh(wmenu);
	Cmd = toupper(getchar());

	switch(Cmd) {
	    case 'Q':
		Screen.CursorOn();
		clear();
		return;
	    case 'R': if (PC != NULL)
		delete PC;
		if (Backgrnd != NULL) {
		    delete Backgrnd;
		    Backgrnd = NULL;
		}
		PC = cPC::Select();
		break;
	    case 'S':
	    	if (PC != NULL)
		    Stats(*PC);
		else
		    Screen.ErrMsg(errNoRace);
		break;
	    case 'B':
		if (PC != NULL) {
		    if (Backgrnd != NULL)
			delete Backgrnd;
		    Backgrnd = cBackgrnd::Select();
		    Backgrnd->SetExp(PC->GetAge() - 15);
		}
		else
		    Screen.ErrMsg(errNoRace);
		break;
	    case 'A':
		if (PC != NULL)	{
		    Age(*PC);
		    if (Backgrnd)
			Backgrnd->SetExp(PC->GetAge() - 15);
		}
		else
		    Screen.ErrMsg(	errNoRace);
		break;
	    case 'F':
		if (PC != NULL)
		    FileSave();
		else {
		    Screen.ErrMsg(	errNoRace);
		    break;
		}
		break;
	    default:
	    	if (Cmd == 0)
		    Cmd = getchar();
		Screen.Beep();
	}
    }
}

//**********************************************************************
//	Ask the user for stats
//**********************************************************************
void cMenu::Stats(cPC & PC) {
    int Row;
    int Col;
    char Ans;

    wclear(wmain);
    Row = Screen.CalcCTextRow(1);
    Col = Screen.CalcCTextCol(txtHasStats);
    wmove(wmain, Row, Col);
    wrefresh(wmain);
    wprintw(wmain,txtHasStats);
    wrefresh(wmain);
    
    do {
    	Ans = getchar();
    	if (Ans != 'y' && Ans != 'n')
	    Screen.Beep();
    } while (Ans != 'y' && Ans != 'n');
    
    wclear(wmain);

    if (Ans == 'y')
	LoadStats(PC);
    else
    	RollStats(PC);
}
//**********************************************************************
//	Input the user specified stats.
//**********************************************************************
void cMenu::LoadStats(cPC & PC) {
    int Row;
    int Val;
    char * Roll;

    Row = Screen.CalcCTextRow(7);

    Roll = PC.GetStrRoll();
    Val = GetValue("Strength", Row++,
		::Dice.RollAvg(Roll),
		::Dice.RollMax(Roll));
    PC.SetStr(Val);

    Roll = PC.GetConRoll();
    Val = GetValue("Constitution", Row++,
		::Dice.RollAvg(Roll),
		::Dice.RollMax(Roll));
    PC.SetCon(Val);

    Roll = PC.GetSizRoll();
    Val = GetValue("Size",Row++,
		::Dice.RollAvg(Roll),
		::Dice.RollMax(Roll));
    PC.SetSiz(Val);

    Roll = PC.GetIntRoll();
    Val = GetValue("Intelligence", Row++,
		::Dice.RollAvg(Roll),
		::Dice.RollMax(Roll));
    PC.SetInt(Val);

    Roll = PC.GetPowRoll();
    Val = GetValue("Power",Row++,
		::Dice.RollAvg(Roll),
		::Dice.RollMax(Roll));
    PC.SetPow(Val);

    Roll = PC.GetDexRoll();
    Val = GetValue("Dexterity",	Row++,
		::Dice.RollAvg(Roll),
		::Dice.RollMax(Roll));
    PC.SetDex(Val);

    Roll = PC.GetAppRoll();
    Val = GetValue("Appearance",Row++,
		::Dice.RollAvg(Roll),
		::Dice.RollMax(Roll));
    PC.SetApp(Val);
}
//**********************************************************************
//	Get a prompted value from the screen.
//**********************************************************************
int cMenu::GetValue(char * Prompt,int Row, int Avg, int Max) {
    int Value;
    int Col;

    Col = Screen.CalcCTextCol("1234567890 (avg=12, max=12): ");
    do {
	wmove(wmain, Row, Col);
	wrefresh(wmain);

	wprintw(wmain, "%15s (avg=%2d, max=%2d): ", Prompt, Avg, Max);
	wrefresh(wmain);
	wscanw(wmain, "%d", &Value);

	if (Value < 1 || Value > Max)
	Screen.ErrMsg(	errBadValue);
    } while (Value < 1 || Value > Max);
    return(Value);
}
//**********************************************************************
//	Roll random stats and ask the user if they are okay.
//**********************************************************************
void cMenu::RollStats(cPC & PC) {
    int Row;
    int Col;
    int CurRow;
    char Ans;
    char * Roll;

    Row = Screen.CalcCTextRow(9);
    Col = Screen.CalcCTextCol("Str = 12 (avg=12, max=12)");
    do { 
	PC.RollStats();
	wmove(wmain, Row, Col);
	wrefresh(wmain);

	CurRow = Row;

	Roll = PC.GetStrRoll();
	wmove(wmain, CurRow++, Col);
	wrefresh(wmain);

	wprintw(wmain, "Str = %2d (avg=%2d, max=%2d)",PC.GetStr(), 
			::Dice.RollAvg(Roll),
			::Dice.RollMax(Roll));
	Roll = PC.GetConRoll();
	wmove(wmain, CurRow++, Col);
	wrefresh(wmain);
	wprintw(wmain, "Con = %2d (avg=%2d, max=%2d)",PC.GetCon(),
			::Dice.RollAvg(Roll),
			::Dice.RollMax(Roll));
	Roll = PC.GetSizRoll();
	wmove(wmain, CurRow++, Col);
	wrefresh(wmain);
	wprintw(wmain,"Siz = %2d (avg=%2d, max=%2d)",PC.GetSiz(),
			::Dice.RollAvg(Roll),
			::Dice.RollMax(Roll));
	wmove(wmain, CurRow++, Col);
	wrefresh(wmain);
	Roll = PC.GetIntRoll();
	wprintw(wmain, "Int = %2d (avg=%2d, max=%2d)",PC.GetInt(),
			::Dice.RollAvg(Roll),
			::Dice.RollMax(Roll));
	Roll = PC.GetPowRoll();
	wmove(wmain, CurRow++, Col);
	wrefresh(wmain);
	wprintw(wmain, "Pow = %2d (avg=%2d, max=%2d)",PC.GetPow(),
			::Dice.RollAvg(Roll),
			::Dice.RollMax(Roll));
	Roll = PC.GetDexRoll();
	wmove(wmain, CurRow++, Col);
	wrefresh(wmain);
	wprintw(wmain, "Dex = %2d (avg=%2d, max=%2d)",PC.GetDex(),
			::Dice.RollAvg(Roll),
			::Dice.RollMax(Roll));
	Roll = PC.GetAppRoll();
	wmove(wmain, CurRow++, Col);
	wrefresh(wmain);
	wprintw(wmain,"App = %2d (avg=%2d, max=%2d)",PC.GetApp(),
			::Dice.RollAvg(Roll),
			::Dice.RollMax(Roll));
	wmove(wmain, CurRow+1, Screen.CalcCTextCol(txtAccept));
	wrefresh(wmain);

	wprintw(wmain, txtAccept);
	Ans = getchar();
	wclear(wmain);
    } while (Ans != 'y');
}
//**********************************************************************
//	Select the character's age.
//**********************************************************************
void cMenu::Age(cPC & PC) {
    int Age;
    int Row;
    int Col;

    Row = Screen.CalcCTextRow(1);
    Col = Screen.CalcCTextCol(txtPCAge);
    do {	
    	Row=21;
	wmove(wmain, Row, Col);
	wrefresh(wmain);
	wprintw(wmain, txtPCAge);
	wscanw(wmain,"%d", &Age);

	if (Age < 15 || Age > 99)
	    Screen.ErrMsg(errBadValue);
    } while (Age < 15 || Age > 99);
    PC.SetAge(Age);
}
//**********************************************************************
//	Save the data to a file
//**********************************************************************
void cMenu::FileSave(void) {
    int	Row;
    int Col;
    char FileName[128];
    FILE * fptr;

    Row = Screen.CalcCTextRow(1);
    Col = Screen.CalcCTextCol(txtFileName);
    Row=21;
    wmove(wmain, Row, Col);
    wrefresh(wmain);
    wprintw(wmain, txtFileName);
    wrefresh(wmain);
    Screen.CursorOn();
    strcpy(FileName, "default");
    wscanw(wmain, "%s", FileName);
    fptr = fopen(FileName, "w");
    if (fptr == NULL)
    	Screen.ErrMsg("ERROR: Failed to open the output file.\n");
    else {
    	PC->Print(fptr);
	if(Backgrnd != NULL)
	    Backgrnd->Print(*PC, fptr);
	fclose(fptr);
    }
}
