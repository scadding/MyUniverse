//**********************************************************************
//	Include Files
//**********************************************************************
#include <stdlib.h>
#include <time.h>

//**********************************************************************
//	Dice Class
//**********************************************************************
class cDice
{
private:
public:
	cDice(void)
	{ srand((unsigned int)time); }

	int Roll(int aNumDice, int aDiceSides);
	int Roll(char *	aRoll);
	int RollAvg(int aNumDice,int aDiceSides);
	int RollAvg(char * aRoll);
	int RollMax(int aNumDice,int aDiceSides);
	int RollMax(char * aRoll);
};

extern cDice Dice;
