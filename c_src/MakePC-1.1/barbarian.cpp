//***************************************************************************
//	Include Files
//***************************************************************************
#include <stdio.h>
#include <ctype.h>

#include "barbarian.h"
#include "skills.h"
#include "screen.h"

//***************************************************************************
//	Static Data
//***************************************************************************
static struct tagSkillsTable SkillsAssistShaman = { "Barbarian Assistant Shaman",
	0,	/* Boat */		0,	/* Climb */		1,	/* Dodge */
	0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
	1,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
	1,	/* Sing */		1,	/* Speak */		3,	/* AnimalLore */
	0,	/* Craft */		0,	/* Evaluate */	3,	/* FirstAid */
	2,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
	3,	/* Plant */		0,	/* Read */		0,	/* Ships */		
	3,	/* World */		0,	/* Conceal */	0,	/* Devise */
	0,	/* Sleight */	0,	/* Instrument */1,	/* Listen */
	0,	/* Scan */		0,	/* Search */	0,	/* Track */
	0,	/* Hide */		0,	/* Sneak */		4,	/* Ceremony */
	4,	/* Summon */	2,	/* Enchant */	2,	/* Fist */
	1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
	1,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsCrafter = { "Barbarian Crafter",
	0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
	0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
	2,	/* Throw */		3,	/* FastTalk */	0,	/* Orate */
	0,	/* Sing */		2,	/* Speak */		1,	/* AnimalLore */
	5,	/* Craft */		3,	/* Evaluate */	1,	/* FirstAid */
	3,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
	0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
	0,	/* World */		2,	/* Conceal */	4,	/* Devise */
	0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
	0,	/* Scan */		0,	/* Search */	0,	/* Track */
	0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
	0,	/* Summon */	1,	/* Enchant */	2,	/* Fist */
	2,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
	1,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsEntertainer = { "Barbarian Entertainer",
	0,	/* Boat */		0,	/* Climb */		1,	/* Dodge */
	0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
	2,	/* Throw */		3,	/* FastTalk */	3,	/* Orate */
	3,	/* Sing */		1,	/* Speak */		0,	/* AnimalLore */
	0,	/* Craft */		0,	/* Evaluate */	2,	/* FirstAid */
	3,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
	0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
	0,	/* World */		2,	/* Conceal */	1,	/* Devise */
	3,	/* Sleight */	3,	/* Instrument */0,	/* Listen */
	0,	/* Scan */		0,	/* Search */	0,	/* Track */
	0,	/* Hide */		0,	/* Sneak */		1,	/* Ceremony */
	0,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
	1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
	0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsFarmer = { "Barbarian Farmer",
	0,	/* Boat */		0,	/* Climb */		2,	/* Dodge */
	0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
	2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
	0,	/* Sing */		0,	/* Speak */		3,	/* AnimalLore */
	2,	/* Craft */		0,	/* Evaluate */	3,	/* FirstAid */
	0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
	5,	/* Plant */		0,	/* Read */		0,	/* Ships */		
	3,	/* World */		0,	/* Conceal */	2,	/* Devise */
	0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
	3,	/* Scan */		2,	/* Search */	0,	/* Track */
	0,	/* Hide */		0,	/* Sneak */		1,	/* Ceremony */
	0,	/* Summon */	0,	/* Enchant */	2,	/* Fist */
	1,	/* Dagger */	2,	/* Wpn1Hand */	0,	/* Wpn2Hand */
	0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsFisher = { "Barbarian Fisher",
	5,	/* Boat */		2,	/* Climb */		1,	/* Dodge */
	0,	/* Jump */		0,	/* Ride */		4,	/* Swim */
	2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
	1,	/* Sing */		0,	/* Speak */		3,	/* AnimalLore */
	0,	/* Craft */		0,	/* Evaluate */	1,	/* FirstAid */
	0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
	1,	/* Plant */		0,	/* Read */		0,	/* Ships */		
	3,	/* World */		0,	/* Conceal */	1,	/* Devise */
	0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
	3,	/* Scan */		0,	/* Search */	0,	/* Track */
	0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
	1,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
	1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
	0,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsHerder = { "Barbarian Herder",
	0,	/* Boat */		2,	/* Climb */		2,	/* Dodge */
	1,	/* Jump */		0,	/* Ride */		0,	/* Swim */
	2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
	0,	/* Sing */		0,	/* Speak */		5,	/* AnimalLore */
	0,	/* Craft */		0,	/* Evaluate */	3,	/* FirstAid */
	0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
	0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
	3,	/* World */		0,	/* Conceal */	0,	/* Devise */
	0,	/* Sleight */	0,	/* Instrument */2,	/* Listen */
	4,	/* Scan */		1,	/* Search */	0,	/* Track */
	0,	/* Hide */		0,	/* Sneak */		0,	/* Ceremony */
	1,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
	1,	/* Dagger */	2,	/* Wpn1Hand */	0,	/* Wpn2Hand */
	2,	/* Shield  */	3,	/* Missile */
};

static struct tagSkillsTable SkillsHunter = { "Barbarian Hunter",
	0,	/* Boat */		1,	/* Climb */		1,	/* Dodge */
	1,	/* Jump */		0,	/* Ride */		0,	/* Swim */
	2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
	0,	/* Sing */		0,	/* Speak */		2,	/* AnimalLore */
	0,	/* Craft */		0,	/* Evaluate */	1,	/* FirstAid */
	0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
	0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
	1,	/* World */		1,	/* Conceal */	2,	/* Devise */
	0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
	2,	/* Scan */		2,	/* Search */	3,	/* Track */
	2,	/* Hide */		3,	/* Sneak */		0,	/* Ceremony */
	1,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
	1,	/* Dagger */	2,	/* Wpn1Hand */	0,	/* Wpn2Hand */
	0,	/* Shield  */	4,	/* Missile */
};

static struct tagSkillsTable SkillsInitiate = { "Barbarian Initiate",
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

static struct tagSkillsTable SkillsNoble = { "Barbarian Noble",
	0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
	0,	/* Jump */		4,	/* Ride */		0,	/* Swim */
	1,	/* Throw */		2,	/* FastTalk */	3,	/* Orate */
	0,	/* Sing */		1,	/* Speak */		0,	/* AnimalLore */
	0,	/* Craft */		2,	/* Evaluate */	1,	/* FirstAid */
	2,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
	0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
	2,	/* World */		0,	/* Conceal */	0,	/* Devise */
	0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
	0,	/* Scan */		1,	/* Search */	0,	/* Track */
	0,	/* Hide */		0,	/* Sneak */		1,	/* Ceremony */
	0,	/* Summon */	0,	/* Enchant */	1,	/* Fist */
	1,	/* Dagger */	4,	/* Wpn1Hand */	4,	/* Wpn2Hand */
	4,	/* Shield  */	2,	/* Missile */
};

static struct tagSkillsTable SkillsPriest = { "Barbarian Priest",
	0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
	0,	/* Jump */		1,	/* Ride */		0,	/* Swim */
	1,	/* Throw */		2,	/* FastTalk */	5,	/* Orate */
	0,	/* Sing */		2,	/* Speak */		0,	/* AnimalLore */
	0,	/* Craft */		0,	/* Evaluate */	1,	/* FirstAid */
	3,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
	0,	/* Plant */		4,	/* Read */		0,	/* Ships */		
	0,	/* World */		0,	/* Conceal */	0,	/* Devise */
	0,	/* Sleight */	0,	/* Instrument */0,	/* Listen */
	0,	/* Scan */		0,	/* Search */	0,	/* Track */
	0,	/* Hide */		0,	/* Sneak */		4,	/* Ceremony */
	2,	/* Summon */	2,	/* Enchant */	1,	/* Fist */
	1,	/* Dagger */	2,	/* Wpn1Hand */	2,	/* Wpn2Hand */
	2,	/* Shield  */	0,	/* Missile */
};

static struct tagSkillsTable SkillsShaman = { "Barbarian Shaman",
	0,	/* Boat */		0,	/* Climb */		1,	/* Dodge */
	0,	/* Jump */		0,	/* Ride */		0,	/* Swim */
	2,	/* Throw */		0,	/* FastTalk */	1,	/* Orate */
	2,	/* Sing */		0,	/* Speak */		3,	/* AnimalLore */
	0,	/* Craft */		0,	/* Evaluate */	2,	/* FirstAid */
	3,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
	3,	/* Plant */		0,	/* Read */		0,	/* Ships */		
	3,	/* World */		0,	/* Conceal */	0,	/* Devise */
	0,	/* Sleight */	1,	/* Instrument */0,	/* Listen */
	0,	/* Scan */		0,	/* Search */	0,	/* Track */
	0,	/* Hide */		0,	/* Sneak */		3,	/* Ceremony */
	3,	/* Summon */	3,	/* Enchant */	1,	/* Fist */
	1,	/* Dagger */	1,	/* Wpn1Hand */	0,	/* Wpn2Hand */
	1,	/* Shield  */	0,	/* Missile */
};
	
static struct tagSkillsTable SkillsWarrior = { "Barbarian Warrior",
	0,	/* Boat */		0,	/* Climb */		0,	/* Dodge */
	0,	/* Jump */		2,	/* Ride */		0,	/* Swim */
	2,	/* Throw */		0,	/* FastTalk */	0,	/* Orate */
	0,	/* Sing */		0,	/* Speak */		0,	/* AnimalLore */
	0,	/* Craft */		0,	/* Evaluate */	2,	/* FirstAid */
	0,	/* RaceLore */	0,	/* MaritalArt */0,	/* Mineral */
	0,	/* Plant */		0,	/* Read */		0,	/* Ships */		
	2,	/* World */		2,	/* Conceal */	0,	/* Devise */
	0,	/* Sleight */	0,	/* Instrument */3,	/* Listen */
	3,	/* Scan */		1,	/* Search */	0,	/* Track */
	1,	/* Hide */		1,	/* Sneak */		0,	/* Ceremony */
	1,	/* Summon */	0,	/* Enchant */	2,	/* Fist */
	1,	/* Dagger */	4,	/* Wpn1Hand */	4,	/* Wpn2Hand */
	4,	/* Shield  */	2,	/* Missile */
};

struct tagSkillsTable cBarbarian::SelectJob(void) {
    int	Choice;

    Choice = Screen.ListBox(12,
				"Assistant Shaman",
				"Crafter         ",
				"Entertainer     ",
				"Farmer          ",
				"Fisher          ",
				"Herder          ",
				"Hunter          ",
				"Initiate        ",
				"Noble           ",
				"Priest          ",
				"Shaman          ",
				"Warrior         ");
    switch(Choice) {
	case 0: return(SkillsAssistShaman);
	case 1: return(SkillsCrafter);
	case 2: return(SkillsEntertainer);
	case 3: return(SkillsFarmer);
	case 4: return(SkillsFisher);
	case 5: return(SkillsHerder);
	case 6: return(SkillsHunter);
	case 7: return(SkillsInitiate);
	case 8: return(SkillsNoble);
	case 9: return(SkillsPriest);
	case 10: return(SkillsShaman);
	case 11: return(SkillsWarrior);
	default: return(SkillsFarmer);
    }
}
