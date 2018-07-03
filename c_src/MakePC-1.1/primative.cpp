//***************************************************************************
//	Include Files
//***************************************************************************
#include <ctype.h>
#include <stdio.h>

#include "skills.h"
#include "primative.h"
#include "screen.h"

//***************************************************************************
//	Tables
//***************************************************************************
struct tagSkillsTable SkillsAssistShaman = { "Primative Assistant Shaman",
		0,	/* Boat */		0,	/* Climb */		1,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		3,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	4,	/* FirstAid */
		1,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		3,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		3,	/* World */		0,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */1,	/* Listen */
		1,	/* Scan */		0,	/* Search */	1,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		3,	/* Ceremony */
		5,	/* Summon */	2,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	1,	/* Wpn1Hand */	1,	/* Wpn2Hand */
		1,	/* Shield  */	0,	/* Missile */
	"water skins, knife, crude furs, leather clothes, beaded goods (30p), \n"
	"firemaker",
};

struct tagSkillsTable SkillsFisherman = { "Primative Fisherman",
		5,	/* Boat */		1,	/* Climb */		1,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		4,	/* Swim */
		3,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		2,	/* AnimalLore */
		3,	/* Craft */		0,	/* Evaluate */	0,	/* FirstAid */
		0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		1,	/* World */		0,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */1,	/* Listen */
		3,	/* Scan */		1,	/* Search */	0,	/* Track */
		1,	/* Hide */		1,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	2,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	2,	/* Missile */
	"50m rope, knife, tattoo, salt fish (120p), leather clothes",
};

struct tagSkillsTable SkillsHunter = { "Primative Hunter",
		0,	/* Boat */		0,	/* Climb */		1,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		3,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		2,	/* AnimalLore */
		1,	/* Craft */		0,	/* Evaluate */	0,	/* FirstAid */
		0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		2,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		0,	/* World */		0,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */3,	/* Listen */
		3,	/* Scan */		0,	/* Search */	3,	/* Track */
		3,	/* Hide */		4,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	2,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	3,	/* Missile */
	"Knife, water skins, firemaker, 50m rope, pack, pelt clothing (120p)",
};

struct tagSkillsTable SkillsShaman = { "Primative Shaman",
		0,	/* Boat */		0,	/* Climb */		1,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		2,	/* Sing */		0,	/* Speak */		3,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	3,	/* FirstAid */
		2,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		2,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		3,	/* World */		0,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */2,	/* Listen */
		2,	/* Scan */		0,	/* Search */	1,	/* Track */
		0,	/* Hide */		1,	/* Sneak */		2,	/* Ceremony */
		2,	/* Summon */	2,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	0,	/* Missile */
	"Water skins, knife, spear, beaded goods (480p), bed roll, firemaker,\n"
	"small drum, leather clothes",
};

struct tagSkillsTable cPrimative::SelectJob(void) {
    int Choice;

    Choice = Screen.ListBox(4,
					"Assistant Shaman",
					"Fisherman       ",
					"Hunter          ",
					"Shaman          ");
    switch (Choice) {
	case 0: return(SkillsAssistShaman);
	case 1: return(SkillsFisherman);
	case 2: return(SkillsHunter);
	case 3: return(SkillsShaman);
	default: return(SkillsHunter);
    }
}
