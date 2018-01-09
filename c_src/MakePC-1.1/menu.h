//**********************************************************************
//	Include Files
//**********************************************************************
#include "pc.h"
#include "backgrnd.h"

//******************************************************************************
//	Class Menu
//******************************************************************************
class cMenu {
	private:
		cPC *	PC;
		cBackgrnd * Backgrnd;

		void Age(cPC & PC);
		void Stats(cPC & PC);
		int GetValue(char * Prompt,int Row,int Avg,int Max);
		void FileSave(void);
		void LoadStats(cPC & PC);
		void RollStats(cPC & PC);

	public:
		cMenu(void);
		void ProcessCmds(void);
};
