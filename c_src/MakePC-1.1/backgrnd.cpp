//**********************************************************************
//	Include Files
//**********************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "pc.h"
#include "backgrnd.h"
#include "screen.h"

#include "dice.h"
#include "primative.h"
#include "barbarian.h"
#include "civilized.h"
#include "nomadic.h"

//**********************************************************************
//	Formatting strings.
//**********************************************************************
char * fmtParry =
"(%3d) Parry          Dagger: %3d   1H Wpn: %3d     2H Wpn: %3d  Shield: %3d\n";

char * fmtAgility =
"(%3d) Agility          Boat: %3d    Climb: %3d      Dodge: %3d    Jump: %3d\n"
"                       Ride: %3d     Swim: %3d      Throw: %3d \n";

char * fmtKnowledge =
"(%3d) Knowledge    Evaluate: %3d    Craft: %3d  First Aid: %3d  Animal: %3d\n"
"                       Race: %3d  Mineral: %3d      Plant: %3d   World: %3d\n"
"                    Kung Fu: %3d     Read: %3d      Ships: %3d \n";

char * fmtManipulate =
"(%3d) Manipulate    Conceal: %3d   Devise: %3d    Sleight: %3d   Music: %3d\n";

char * fmtStealth =
"(%3d) Stealth          Hide: %3d    Sneak: %3d \n";

char * fmtCommunicate =
"(%3d) Communicate  FastTalk: %3d    Orate: %3d       Sing: %3d   Speak: %3d\n";

char * fmtAttack =
"(%3d) Attack           Fist: %3d   Dagger: %3d     1H Wpn: %3d  2H Wpn: %3d\n"
"                    Missile: %3d \n";

char * fmtPerception =
"(%3d) Perception     Listen: %3d     Scan: %3d     Search: %3d   Track: %3d\n";

char * fmtMagical =
"(%3d) Magical      Ceremony: %3d   Summon: %3d    Enchant: %3d \n";

//**********************************************************************
//	Select the culture and job skills.
//**********************************************************************
cBackgrnd * cBackgrnd::Select(void) {
    int Choice;
    cBackgrnd * Backgrnd;

    Choice = Screen.ListBox(4,
				"Primative",
				"Nomadic  ",
				"Barbarian",
				"Civilized");
    switch(Choice) {
    	case 0:
		Backgrnd = new cBackgrnd(cPrimative::SelectJob());
		return(Backgrnd);
	case 1:
		Backgrnd = new cBackgrnd(cNomadic::SelectJob());
		return(Backgrnd);
	case 2:
		Backgrnd = new cBackgrnd(cBarbarian::SelectJob());
		return(Backgrnd);
	case 3:
		Backgrnd = new cBackgrnd(cCivilized::SelectJob());
		return(Backgrnd);
    }
    return (0);
}
//**********************************************************************
//	Output the data.
//**********************************************************************
void cBackgrnd::Output (cPC & PC) {
	int V1, V2, V3, V4, V5, V6, V7, V8, V9;

	PC.GetSkillBonuses(V1, V2, V3, V4, V5, V6, V7, V8, V9);

	wprintw(wmain,
				"%s\n",
				GetName());

	wprintw(wmain,
				fmtAgility, 
				V1,
				CheckSkillLevel(GetBoat(), V1),
				CheckSkillLevel(GetClimb(), V1),
				CheckSkillLevel(GetDodge(), V1),
				CheckSkillLevel(GetJump(), V1),
				CheckSkillLevel(GetRide(), V1),
				CheckSkillLevel(GetSwim(), V1),
				CheckSkillLevel(GetThrow(), V1));

	wprintw(wmain,
				fmtKnowledge,
				V2,
				CheckSkillLevel(GetEvaluate(), V2),
				CheckSkillLevel(GetCraft(), V2),
				CheckSkillLevel(GetFirstAid(), V2),
				CheckSkillLevel(GetAnimal(), V2),
				CheckSkillLevel(GetRace(), V2),
				CheckSkillLevel(GetMineral(), V2),
				CheckSkillLevel(GetPlant(), V2),
				CheckSkillLevel(GetWorld(), V2),
				CheckSkillLevel(GetMartialArt(), V2),
				CheckSkillLevel(GetRead(), V2),
				CheckSkillLevel(GetShips(), V2));

	wprintw(wmain,
				fmtManipulate,
				V3,
				CheckSkillLevel(GetConceal(), V3),
				CheckSkillLevel(GetDevise(), V3),
				CheckSkillLevel(GetSleight(), V3),
				CheckSkillLevel(GetInstrument(), V3));

	wprintw(wmain,
				fmtStealth,
				V4,
				CheckSkillLevel(GetHide(), V4),
				CheckSkillLevel(GetSneak(), V4));

	wprintw(wmain,
				fmtCommunicate,
				V5,
				CheckSkillLevel(GetFastTalk(), V5),
				CheckSkillLevel(GetOrate(), V5),
				CheckSkillLevel(GetSing(), V5),
				CheckSkillLevel(GetSpeak(), V5));

	wprintw(wmain,
				fmtMagical,
				V6,
				CheckSkillLevel(GetCeremony(), V6),
				CheckSkillLevel(GetSummon(), V6),
				CheckSkillLevel(GetEnchant(), V6));

	wprintw(wmain,
				fmtPerception,
				V7,
				CheckSkillLevel(GetListen(), V7),
				CheckSkillLevel(GetScan(), V7),
				CheckSkillLevel(GetSearch(), V7),
				CheckSkillLevel(GetTrack(), V7));

	wprintw(wmain,
				fmtAttack,
				V8,
				CheckSkillLevel(GetFist(), V8),
				CheckSkillLevel(GetDagger(), V8),
				CheckSkillLevel(GetWpn1Hand(), V8),
				CheckSkillLevel(GetWpn2Hand(), V8),
				CheckSkillLevel(GetMissile(), V8));

	wprintw(wmain,
				fmtParry,
				V9,
				CheckSkillLevel(GetDagger(), V9),
				CheckSkillLevel(GetWpn1Hand(), V9),
				CheckSkillLevel(GetWpn2Hand(), V9),
				CheckSkillLevel(GetShield(), V9));
	wrefresh(wmain);
}

void cBackgrnd::Print(cPC & PC, FILE * fptr) {
	int	V1, V2, V3, V4, V5, V6, V7, V8, V9;

	PC.GetSkillBonuses(V1, V2, V3, V4, V5, V6, V7, V8, V9);

	fprintf(fptr,
				"%s\n",
				GetName());

	fprintf(fptr,
				fmtAgility, 
				V1,
				CheckSkillLevel(GetBoat(), V1),
				CheckSkillLevel(GetClimb(), V1),
				CheckSkillLevel(GetDodge(), V1),
				CheckSkillLevel(GetJump(), V1),
				CheckSkillLevel(GetRide(), V1),
				CheckSkillLevel(GetSwim(), V1),
				CheckSkillLevel(GetThrow(), V1));

	fprintf(fptr,
				fmtKnowledge,
				V2,
				CheckSkillLevel(GetEvaluate(), V2),
				CheckSkillLevel(GetCraft(), V2),
				CheckSkillLevel(GetFirstAid(), V2),
				CheckSkillLevel(GetAnimal(), V2),
				CheckSkillLevel(GetRace(), V2),
				CheckSkillLevel(GetMineral(), V2),
				CheckSkillLevel(GetPlant(), V2),
				CheckSkillLevel(GetWorld(), V2),
				CheckSkillLevel(GetMartialArt(), V2),
				CheckSkillLevel(GetRead(), V2),
				CheckSkillLevel(GetShips(), V2));

	fprintf(fptr,
				fmtManipulate,
				V3,
				CheckSkillLevel(GetConceal(), V3),
				CheckSkillLevel(GetDevise(), V3),
				CheckSkillLevel(GetSleight(), V3),
				CheckSkillLevel(GetInstrument(), V3));

	fprintf(fptr,
				fmtStealth,
				V4,
				CheckSkillLevel(GetHide(), V4),
				CheckSkillLevel(GetSneak(), V4));

	fprintf(fptr,
				fmtCommunicate,
				V5,
				CheckSkillLevel(GetFastTalk(), V5),
				CheckSkillLevel(GetOrate(), V5),
				CheckSkillLevel(GetSing(), V5),
				CheckSkillLevel(GetSpeak(), V5));

	fprintf(fptr,
				fmtMagical,
				V6,
				CheckSkillLevel(GetCeremony(), V6),
				CheckSkillLevel(GetSummon(), V6),
				CheckSkillLevel(GetEnchant(), V6));

	fprintf(fptr,
				fmtPerception,
				V7,
				CheckSkillLevel(GetListen(), V7),
				CheckSkillLevel(GetScan(), V7),
				CheckSkillLevel(GetSearch(), V7),
				CheckSkillLevel(GetTrack(), V7));

	fprintf(fptr,
				fmtAttack,
				V8,
				CheckSkillLevel(GetFist(), V8),
				CheckSkillLevel(GetDagger(), V8),
				CheckSkillLevel(GetWpn1Hand(), V8),
				CheckSkillLevel(GetWpn2Hand(), V8),
				CheckSkillLevel(GetMissile(), V8));

	fprintf(fptr,
				fmtParry,
				V9,
				CheckSkillLevel(GetDagger(), V9),
				CheckSkillLevel(GetWpn1Hand(), V9),
				CheckSkillLevel(GetWpn2Hand(), V9),
				CheckSkillLevel(GetShield(), V9));
}


//**********************************************************************
//	Check the skill level so it doesn't exceed maximum possible.
//**********************************************************************
int cBackgrnd::CheckSkillLevel(int Score, int Bonus) {
	if (Score > 0)
		Score += Bonus;

	if (Score > 100 + Bonus)
		Score = 100 + Bonus;

	if (Score < 0)
		Score = 0;

	return(Score);
}

