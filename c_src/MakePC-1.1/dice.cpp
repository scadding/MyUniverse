//**********************************************************************
//	Include Files
//**********************************************************************
#include <stdio.h>
#include "dice.h"

//**********************************************************************
//	Roll the dice
//**********************************************************************
int cDice::Roll(char * aRoll) {
    int	NumDice = 0;
    int	DiceSides = 0;
    int	Bonus = 0;

    sscanf(aRoll, "%dd%d+%d", &NumDice, &DiceSides, &Bonus);
    return(Roll(NumDice, DiceSides) + Bonus);
}


//**********************************************************************
//	Roll the dice
//**********************************************************************
int cDice::Roll(int aNumDice, int aDiceSides) {
    int	Value = 0;

    while (aNumDice--) {
	Value += rand()%(aDiceSides+1) + 1;
    }
    return(Value);
}

//**********************************************************************
//	Roll the dice
//**********************************************************************
int cDice::RollAvg(char * aRoll) {
    int	NumDice = 0;
    int DiceSides = 0;
    int Bonus = 0;

    sscanf(aRoll,"%dd%d+%d", &NumDice, &DiceSides, &Bonus);
    return(RollAvg(NumDice, DiceSides) + Bonus);
}


//**********************************************************************
//	Roll the dice
//**********************************************************************
int cDice::RollAvg(int aNumDice,int aDiceSides) {
    return((aNumDice * (1 + aDiceSides) + 1) / 2);
}

//**********************************************************************
//	Roll the dice
//**********************************************************************
int cDice::RollMax(char * aRoll) {
    int NumDice = 0;
    int DiceSides = 0;
    int Bonus = 0;

    sscanf(aRoll,"%dd%d+%d",&NumDice,&DiceSides,&Bonus);
    return(RollMax(NumDice, DiceSides) + Bonus);
}

//**********************************************************************
//	Roll the dice
//**********************************************************************
int cDice::RollMax(int aNumDice, int aDiceSides) {
    return(aNumDice * aDiceSides);
}

