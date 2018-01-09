//**********************************************************************
//	Include Files
//**********************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dice.h"
#include "pc.h"
#include "backgrnd.h"
#include "screen.h"

char * fmtRaceInfo =
"Race: %-20s   Age: %2d  Dex SRM: %2d  Siz SRM: %2d\n";
char * fmtOtherPoints =
"Magic Points: %2d  Fatigue Points: %2d  Damage Modifier: %s\n";
char * fmtHitPoints =
"  Hit Points: %2d  Head: %2d  Chest: %2d  Arms: %2d  Legs: %2d\n";
char * fmtStats =
"STR: %2d  CON: %2d  SIZ: %2d  INT: %2d  POW: %2d  DEX: %2d  APP: %2d\n\n";

//**********************************************************************
//	Race Tables
//**********************************************************************
struct tagRaceTable HumanRaceTable = { "Human",
	"3d6+0", "3d6+0", "2d6+6", "2d6+6",
	"3d6+0", "3d6+0", "3d6+0"
};

struct tagRaceTable ElfRaceTable = { "Elf",
	"2d6+2", "3d6+0", "2d4+4", "3d6+6",
	"2d6+6", "3d6+3", "3d6"
};

struct tagRaceTable DwarfRaceTable = { "Dwarf",
	"4d6+0", "1d6+12", "2d6+0", "2d6+6",
	"3d6+0", "3d6+0", "3d6+0"
};

struct tagRaceTable HobbitRaceTable = { "Hobbit",
	"2d6+0", "2d6+12", "2d3+0", "2d6+6",
	"2d6+3", "2d6+10", "3d6+0"
};

struct tagRaceTable DarkTrollRaceTable = { "Dark Troll",
	"3d6+6", "3d6+0", "3d6+8", "2d6+6",
	"3d6+0", "3d6+0", "3d6+0"
};

struct tagRaceTable BrooRaceTable = { "Broo", 
	"2d6+6", "1d6+12", "1d6+12", "2d6+6",
	"3d6+0", "3d6+0", "3d6+0"
};

struct tagRaceTable StoneTrollRaceTable = { "Stone Troll",
	"3d6+24", "3d6+24", "2d6+32", "1d6+6",
	"3d6+0", "2d6+0", "3d6+0"
};

struct tagRaceTable UrukRaceTable = { "Uruk",
	"4d6+0", "3d6+0", "2d6+8", "2d6+4",
	"2d6+3", "3d6+0", "2d6+0"
};

//*****************************************************************************
//	Select a race and construct the object for it.
//*****************************************************************************
cPC * cPC::Select(void) {
	int Choice;

	Choice = Screen.ListBox(8,
				"Human     ",
				"Elf       ",
				"Dwarf     ",
				"Hobbit    ",
				"Dark Troll",
				"Broo      ",
				"Stone Troll",
				"Uruk");
	switch(Choice)
	{
		case 0: return(new cPC(HumanRaceTable));
		case 1: return(new cPC(ElfRaceTable));
		case 2: return(new cPC(DwarfRaceTable));
		case 3: return(new cPC(HobbitRaceTable));
		case 4: return(new cPC(DarkTrollRaceTable));
		case 5: return(new cPC(BrooRaceTable));
		case 6: return(new cPC(StoneTrollRaceTable));
		case 7: return(new cPC(UrukRaceTable));
		default: return(new cPC(HumanRaceTable));
	}
}

//**********************************************************************
//	Construct the PC object.
//**********************************************************************
cPC::cPC(struct tagRaceTable RaceTable)
{
	Age = 15 + ::Dice.RollAvg(2, 6);
	Race = RaceTable;
	RollAvgStats();
}

//**********************************************************************
//	Roll the object's stats.
//**********************************************************************
void cPC::RollStats(void) {
	Str = ::Dice.Roll(Race.StrRoll);
	Con = ::Dice.Roll(Race.ConRoll);
	Siz = ::Dice.Roll(Race.SizRoll);
	Int = ::Dice.Roll(Race.IntRoll);
	Pow = ::Dice.Roll(Race.PowRoll);
	Dex = ::Dice.Roll(Race.DexRoll);
	App = ::Dice.Roll(Race.AppRoll);
}

//**********************************************************************
//	Roll the object's stats.
//**********************************************************************
void cPC::RollAvgStats(void) {
	Str = ::Dice.RollAvg(Race.StrRoll);
	Con = ::Dice.RollAvg(Race.ConRoll);
	Siz = ::Dice.RollAvg(Race.SizRoll);
	Int = ::Dice.RollAvg(Race.IntRoll);
	Pow = ::Dice.RollAvg(Race.PowRoll);
	Dex = ::Dice.RollAvg(Race.DexRoll);
	App = ::Dice.RollAvg(Race.AppRoll);
}

//**********************************************************************
//	Get the hit points
//**********************************************************************
void cPC::GetHitPoints(int &Total,int &Head, int &Chest,int &Arm,int &Abs,int &Leg) {
	Total = (GetSiz() + GetCon() + 1) / 2;
	if (Total < 4) 	{
		Head = 1; Chest = 2; Abs = 1; Arm = 1; Leg = 1;
	}
	else if (Total < 7) {
		Head = 2; Chest = 3; Abs = 2; Arm = 2; Leg = 2;
	}
	else if (Total < 10) {
		Head = 3; Chest = 4; Abs = 3; Arm = 3; Leg = 3;
	}
	else if (Total < 13) {
		Head = 4; Chest = 5; Abs = 4; Arm = 3; Leg = 4;
	}
	else if (Total < 16) {
		Head = 5; Chest = 6; Abs = 5; Arm = 4; Leg = 5;
	}
	else if (Total < 19) {
		Head = 6; Chest = 8; Abs = 6; Arm = 5; Leg = 6;
	}
	else if (Total < 22) {
		Head = 7; Chest = 9; Abs = 7; Arm = 6; Leg = 7;
	}
	else {
		Head = (int)((float)Total * 0.33 + 0.5);
		Chest = (int)((float)Total * 0.40 + 0.5);
		Abs = (int)((float)Total * 0.33 + 0.5);
		Arm = (int)((float)Total * 0.25 + 0.5);
		Leg = (int)((float)Total * 0.33 + 0.5);
	}
}

//**********************************************************************
//	Get the damage modifier.
//**********************************************************************
char * cPC::GetDamageModifier(void) {
	int Total, Additional;
	char Text[6];

	Total = GetStr() + GetSiz();
	if (Total < 13)
		return("-1d4");
	else if (Total < 25)
		return("0");
	else if (Total < 33)
		return("1d4");
	else if (Total < 41)
		return("1d6");
	else if (Total < 57)
		return("2d6");
	else {
		Additional = (int)((Total - 57 + 15) / 16)+2;
		sprintf(Text, "%.2dd6", Additional);
		return(Text);
	}
}

//**********************************************************************
//	Get the dexterity strike rank modifier.
//**********************************************************************
int	cPC::GetDexSRM(void) {
	int	Dex;

	Dex = GetDex();
	if (Dex < 10)
		return(4);
	else if (Dex < 16)
		return(3);
	else if (Dex < 20)
		return(2);
	else
		return(1);
}

//**********************************************************************
//	Get the size strike rank modifier.
//**********************************************************************
int	cPC::GetSizSRM(void) {
	int	Siz;

	Siz = GetSiz();
	if (Siz < 10)
		return(3);
	else if (Siz < 16)
		return(2);
	else if (Siz < 20)
		return(1);
	else
		return(0);
}

//**********************************************************************
//	Get the skill class bonuses.
//**********************************************************************
void cPC::GetSkillBonuses(
	int &	Agility,
	int &	Knowledge,
	int &	Manipulate,
	int &	Stealth,
	int &	Communicate,
	int &	Magical,
	int &	Perception,
	int &	Attack,
	int &	Parry)
{
	Agility = GetPrimary(GetDex()) + GetSecondary(GetStr())
		+ GetNegative(GetSiz());
	Knowledge = GetPrimary(GetInt());
	Manipulate = GetPrimary(GetInt()) + GetPrimary(GetDex())
		+ GetSecondary(GetStr());
	Stealth = GetPrimary(GetDex()) + GetNegative(GetSiz())
		+ GetNegative(GetPow());
	Communicate = GetPrimary(GetInt()) + GetSecondary(GetPow())
		+ GetSecondary(GetApp());
	Magical = GetPrimary(GetInt()) + GetPrimary(GetPow())
		+ GetSecondary(GetDex());
	Perception = GetPrimary(GetInt()) + GetSecondary(GetPow())
		+ GetSecondary(GetCon());
	Attack = Manipulate;
	Parry = Agility;
}

//**********************************************************************
//	Output the PC data to the screen
//**********************************************************************
void cPC::Output() {
    int V1, V2, V3, V4, V5, V6;
    wprintw(wmain,
		fmtRaceInfo,
		GetName(),
		GetAge(),
		GetDexSRM(),
		GetSizSRM());

    wprintw(wmain,
		fmtOtherPoints,
		GetMagicPoints(),
		GetFatiguePoints(),
		GetDamageModifier());
	        GetHitPoints(V1, V2, V3, V4, V5, V6);
	
    wprintw(wmain,
		fmtHitPoints,
		V1,
		V2,
		V3,
		V4,
		V5,
		V6);

    wprintw(wmain,
		fmtStats,
		GetStr(),
		GetCon(),
		GetSiz(),
		GetInt(),
		GetPow(),
		GetDex(),
		GetApp());
    wrefresh(wmain);
}

void cPC::Print(FILE * fptr) {
    int V1, V2, V3, V4, V5, V6;
    fprintf(fptr,
		fmtRaceInfo,
		GetName(),
		GetAge(),
		GetDexSRM(),
		GetSizSRM());

    fprintf(fptr,
		fmtOtherPoints,
		GetMagicPoints(),
		GetFatiguePoints(),
		GetDamageModifier());
	        GetHitPoints(V1, V2, V3, V4, V5, V6);
	
    fprintf(fptr,
		fmtHitPoints,
		V1,
		V2,
		V3,
		V4,
		V5,
		V6);

    fprintf(fptr,
		fmtStats,
		GetStr(),
		GetCon(),
		GetSiz(),
		GetInt(),
		GetPow(),
		GetDex(),
		GetApp());
}

