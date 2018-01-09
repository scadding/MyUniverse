//***************************************************************************
//	Include Files
//***************************************************************************
#include <stdio.h>
#include <ctype.h>

#include "nomadic.h"
#include "skills.h"
#include "screen.h"

//***************************************************************************
//	Static Data
//***************************************************************************
static struct tagSkillsTable SkillsAssistShaman =
{
		"Nomadic Assistant Shaman",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		0,	/* Jump */		2,	/* Ride */		0,	/* Swim */
		1,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		3,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	3,	/* FirstAid */
		1,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		3,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		3,	/* World */		0,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */1,	/* Listen */
		1,	/* Scan */		0,	/* Search */	1,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		3,	/* Ceremony */
		5,	/* Summon */	2,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		1,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsCrafter =
{
		"Nomadic Crafter",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		0,	/* Jump */		3,	/* Ride */		0,	/* Swim */
		1,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		2,	/* Speak */		0,	/* AnimalLore */
		5,	/* Craft */		3,	/* Evaluate */	1,	/* FirstAid */
		2,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		1,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		0,	/* World */		1,	/* Conceal */	2,	/* Devise */
		1,	/* Sleight */	0,	/* Instrument */1,	/* Listen */
		1,	/* Scan */		2,	/* Search */	1,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	1,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		1,	/* Shield  */	1,	/* Missile */
};

static struct tagSkillsTable SkillsHerder =
{
		"Nomadic Herder",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		1,	/* Jump */		5,	/* Ride */		0,	/* Swim */
		1,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		3,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	1,	/* FirstAid */
		0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		2,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		0,	/* World */		1,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */2,	/* Listen */
		4,	/* Scan */		1,	/* Search */	3,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		1,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	2,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		2,	/* Shield  */	2,	/* Missile */
};

static struct tagSkillsTable SkillsHunter =
{
		"Nomadic Hunter",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		0,	/* Jump */		5,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		3,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	1,	/* FirstAid */
		0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		1,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		0,	/* World */		1,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */1,	/* Listen */
		3,	/* Scan */		2,	/* Search */	3,	/* Track */
		1,	/* Hide */		1,	/* Sneak */		0,	/* Ceremony */
		1,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	2,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		1,	/* Shield  */	3,	/* Missile */
};

static struct tagSkillsTable SkillsNoble =
{
		"Nomadic Noble",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		0,	/* Jump */		5,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		0,	/* FastTalk */	3,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		0,	/* AnimalLore */
		0,	/* Craft */		1,	/* Evaluate */	1,	/* FirstAid */
		2,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		1,	/* World */		0,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	1,	/* Instrument */1,	/* Listen */
		2,	/* Scan */		2,	/* Search */	1,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		1,	/* Ceremony */
		0,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	3,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		3,	/* Shield  */	2,	/* Missile */
};

static struct tagSkillsTable SkillsShaman =
{
		"Nomadic Shaman",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		0,	/* Jump */		2,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		2,	/* Sing */		0,	/* Speak */		3,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	3,	/* FirstAid */
		2,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		1,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		2,	/* World */		0,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */1,	/* Listen */
		1,	/* Scan */		0,	/* Search */	1,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		3,	/* Ceremony */
		3,	/* Summon */	3,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		1,	/* Shield  */	1,	/* Missile */
};

static struct tagSkillsTable SkillsWarrior =
{
		"Nomadic Warrior",
		0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
		0,	/* Jump */		5,	/* Ride */		0,	/* Swim */
		2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
		0,	/* Sing */		0,	/* Speak */		2,	/* AnimalLore */
		0,	/* Craft */		0,	/* Evaluate */	2,	/* FirstAid */
		0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
		0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
		0,	/* World */		2,	/* Conceal */	0,	/* Devise */
		0,	/* Sleight */	0,	/* Instrument */2,	/* Listen */
		3,	/* Scan */		1,	/* Search */	0,	/* Track */
		0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
		0,	/* Summon */	1,	/* Enchant */	1,	/* Fist */
		1,	/* Dagger */	4,	/* Wpn1Hand */	0,	/* Wpn2Hand */
		3,	/* Shield  */	4,	/* Missile */
};

struct tagSkillsTable cNomadic::SelectJob(void) {
    int Choice;

    Choice = Screen.ListBox(7,
				"Assistant Shaman",
				"Crafter         ",
				"Herder          ",
				"Hunter          ",
				"Noble           ",
				"Shaman          ",
				"Warrior         ");
    switch(Choice) {
	case 0: return(SkillsAssistShaman);
	case 1: return(SkillsCrafter);
	case 2: return(SkillsHerder);
	case 3: return(SkillsHunter);
	case 4: return(SkillsNoble);
	case 5: return(SkillsShaman);
	case 6: return(SkillsWarrior);
	default: return(SkillsHerder);
    }
}
