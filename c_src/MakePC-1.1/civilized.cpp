//***************************************************************************
//	Include Files
//***************************************************************************
#include <stdio.h>
#include <ctype.h>

#include "civilized.h"
#include "skills.h"
#include "screen.h"

//***************************************************************************
//	Static Data
//***************************************************************************
static struct tagSkillsTable SkillsAdept =
{
		"Civilized Adept",
		0,	/* Boat */		0,	/* Climb */		2,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		0,	/* Throw */		2,	/* FastTalk */	2,	/* Orate */
		1,	/* Sing */		2,	/* Speak */		0,	/* AnimalLore */
		1,	/* Craft */		1,	/* Evaluate */	0,	/* FirstAid */
		0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		2,	/* Read */		0,	/* Ships */		
		4,	/* World */		0,	/* Conceal */	2,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
		0,	/* Scan */		0,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	0,	/* Fist */
		0,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsApprentice =
{
		"Civilized Apprentice",
		0,	/* Boat */		0,	/* Climb */		1,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		1,	/* Throw */		0,	/* FastTalk */	2,	/* Orate */
		2,	/* Sing */		1,	/* Speak */		0,	/* AnimalLore */
		1,	/* Craft */		0,	/* Evaluate */	0,	/* FirstAid */
		0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		3,	/* Read */		0,	/* Ships */		
		3,	/* World */		0,	/* Conceal */	2,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
		0,	/* Scan */		0,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
		0,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsCrafter =
{
		"Civilized Crafter",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		2,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		1,	/* Speak */		0,	/* AnimalLore */
		5,	/* Craft */		4,	/* Evaluate */	0,	/* FirstAid */
		2,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		0,	/* World */		1,	/* Conceal */	5,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
		1,	/* Scan */		2,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	2,	/* Fist */
		1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsEntertainer =
{
		"Civilized Entertainer",
		0,	/* Boat */		0,	/* Climb */		1,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		2,	/* FastTalk */	2,	/* Orate */
		3,	/* Sing */		1,	/* Speak */		0,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	0,	/* FirstAid */
		4,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		0,	/* World */		3,	/* Conceal */	1,	/* Devise */
		4,	/* Sleight */	5,	/* Instrument */0,	/* Listen */
		0,	/* Scan */		0,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	2,	/* Fist */
		1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsFarmer =
{
		"Civilized Farmer",
		0,	/* Boat */		1,	/* Climb */		1,	/* Dodge */
		1,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		3,	/* AnimalLore */
		1,	/* Craft */		0,	/* Evaluate */	2,	/* FirstAid */
		2,	/* RaceLore */	0,	/* MaritalArt */2,	/* Mineral */
		4,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		3,	/* World */		0,	/* Conceal */	2,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */1,	/* Listen */
		2,	/* Scan */		1,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	2,	/* Fist */
		1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsHealer =
{
		"Civilized Healer",
		0,	/* Boat */		0,	/* Climb */		1,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		1,	/* Throw */		0,	/* FastTalk */	1,	/* Orate */
		0,	/* Sing */		1,	/* Speak */		3,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	5,	/* FirstAid */
		4,	/* RaceLore */	0,	/* MaritalArt */3,	/* Mineral */
		5,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		0,	/* World */		0,	/* Conceal */	3,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
		0,	/* Scan */		2,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsHerder =
{
		"Civilized Herder",
		0,	/* Boat */		2,	/* Climb */		1,	/* Dodge */
		2,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		5,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	2,	/* FirstAid */
		1,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		3,	/* World */		0,	/* Conceal */	2,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */3,	/* Listen */
		3,	/* Scan */		1,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	2,	/* Fist */
		1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	2,	/* Missile */
};

static struct tagSkillsTable SkillsInitiate =
{
		"Civilized Initiate",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		0,	/* Throw */		0,	/* FastTalk */	1,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		0,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	0,	/* FirstAid */
		0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		1,	/* Read */		0,	/* Ships */		
		0,	/* World */		0,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
		0,	/* Scan */		0,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		1,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	0,	/* Fist */
		0,	/* Dagger */	0,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsMerchant =
{
		"Civilized Merchant",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		4,	/* FastTalk */	2,	/* Orate */
		0,	/* Sing */		3,	/* Speak */		0,	/* AnimalLore */
		0,	/* Craft */		5,	/* Evaluate */	1,	/* FirstAid */
		4,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		2,	/* Read */		0,	/* Ships */		
		0,	/* World */		0,	/* Conceal */	2,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
		0,	/* Scan */		1,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	0,	/* Fist */
		1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsNoble =
{
		"Civilized Noble",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		0,	/* Jump */		4,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		3,	/* FastTalk */	3,	/* Orate */
		0,	/* Sing */		3,	/* Speak */		0,	/* AnimalLore */
		0,	/* Craft */		2,	/* Evaluate */	0,	/* FirstAid */
		3,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		3,	/* World */		0,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
		0,	/* Scan */		0,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	2,	/* Fist */
		1,	/* Dagger */	3,	/* Wpn1Hand */	3,	/* Wpn2Hand */
		2,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsPriest =
{
		"Civilized Priest",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		0,	/* Jump */		3,	/* Ride */		0,	/* Swim */
		1,	/* Throw */		2,	/* FastTalk */	5,	/* Orate */
		0,	/* Sing */		3,	/* Speak */		0,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	1,	/* FirstAid */
		3,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		3,	/* Read */		0,	/* Ships */		
		0,	/* World */		0,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
		0,	/* Scan */		0,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		3,	/* Ceremony */
		2,	/* Summon */	2,	/* Enchant */	2,	/* Fist */
		1,	/* Dagger */	1,	/* Wpn1Hand */	1,	/* Wpn2Hand */
		1,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsSailer =
{
		"Civilized Sailer",
		5,	/* Boat */		3,	/* Climb */		1,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		3,	/* Swim */
		2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		0,	/* AnimalLore */
		2,	/* Craft */		0,	/* Evaluate */	0,	/* FirstAid */
		2,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		0,	/* Read */		3,	/* Ships */		
		3,	/* World */		0,	/* Conceal */	2,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
		2,	/* Scan */		0,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	2,	/* Fist */
		1,	/* Dagger */	2,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsScribe =
{
		"Civilized Scribe",
		0,	/* Boat */		0,	/* Climb */		1,	/* Dodge */
		0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		1,	/* Throw */		1,	/* FastTalk */	1,	/* Orate */
		0,	/* Sing */		3,	/* Speak */		0,	/* AnimalLore */
		2,	/* Craft */		2,	/* Evaluate */	0,	/* FirstAid */
		3,	/* RaceLore */	0,	/* MaritalArt */2,	/* Mineral */
		0,	/* Plant */		3,	/* Read */		0,	/* Ships */		
		3,	/* World */		0,	/* Conceal */	3,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
		0,	/* Scan */		2,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	0,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsSoldier =
{
		"Civilized Soldier",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		0,	/* Jump */		2,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		0,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	3,	/* FirstAid */
		3,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		0,	/* World */		1,	/* Conceal */	1,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */2,	/* Listen */
		2,	/* Scan */		0,	/* Search */	0,	/* Track */
		1,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	2,	/* Fist */
		2,	/* Dagger */	4,	/* Wpn1Hand */	2,	/* Wpn2Hand */
		4,	/* Shield  */	4,	/* Missile */
};

static struct tagSkillsTable SkillsThief =
{
		"Civilized Thief",
		0,	/* Boat */		3,	/* Climb */		2,	/* Dodge */
		1,	/* Jump */		0,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		2,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		0,	/* AnimalLore */
		0,	/* Craft */		3,	/* Evaluate */	0,	/* FirstAid */
		2,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		0,	/* World */		3,	/* Conceal */	2,	/* Devise */
		3,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
		0,	/* Scan */		0,	/* Search */	0,	/* Track */
		3,	/* Hide */		2,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	2,	/* Fist */
		1,	/* Dagger */	2,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		0,	/* Shield  */	0,	/* Missile */
};

struct tagSkillsTable cCivilized::SelectJob(void) {
    int Choice;

    Choice = Screen.ListBox(15,
					"Apprentice ",
					"Crafter    ",
					"Entertainer",
					"Farmer     ",
					"Healer     ",
					"Herder     ",
					"Merchant   ",
					"Noble      ",
					"Initiate   ",
					"Sailer     ",
					"Scribe     ",
					"Soldier    ",
					"Thief      ",
					"Adept      ",
					"Priest     ");
    switch(Choice) {
	case 0: return(SkillsApprentice);
	case 1: return(SkillsCrafter);
	case 2: return(SkillsEntertainer);
	case 3: return(SkillsFarmer);
	case 4: return(SkillsHealer);
	case 5: return(SkillsHerder);
	case 6: return(SkillsMerchant);
	case 7: return(SkillsNoble);
	case 8: return(SkillsInitiate);
	case 9: return(SkillsSailer);
	case 10: return(SkillsScribe);
	case 11: return(SkillsSoldier);
	case 12: return(SkillsThief);
	case 13: return(SkillsAdept);
	case 14: return(SkillsPriest);
	default: return(SkillsFarmer);
    }
}
