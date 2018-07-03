#include <ncurses.h>

//**********************************************************************
//	Screen Class
//**********************************************************************
class cScreen {
    private:
    	int ProcessArrowKey(int FirstRow,int CurRow,int NumRows,int Col);
    public:
    	void CursorOff(void);
	void CursorOn(void);
	void Beep(void);
	void Clear(void);
	void Go(int Row);
	void Go(int Row,int Col);
	void ErrMsg(char * Msg);
	int ListBox(int ItemCnt,...);
	int CalcCTextRow(int NumLines);
	int CalcCTextCol(char * Text);
};		

extern cScreen Screen;

extern WINDOW *wmain;
extern WINDOW *wstat;
extern WINDOW *wmenu;

#define C_WHITE_BLUE    14
#define C_YELLOW_RED     13
