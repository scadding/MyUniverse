struct tagSpell {
	char * Name;
	enum {
		typVariable,
		typFixed,
	} Type;
	int Points;
	enum {
		typOffensive,
		typDefensive,
		typEnhancement,
		typMisc,
		typRitual,
	} Category;
};

#define defSpellCnt		45
struct tagSpell SpellTable[defSpellCnt] = {
	{ "Befuddle",typFixed, 2,typOffensive },
	{ "Demoralize",typFixed,2,typOffensive },
	{ "Disruption",typFixed,1,typOffensive },
	{ "Bladesharp",typVariable,0,typOffensive },
	{ "Bludgeon",typVariable,0,typOffensive },
	{ "Firearrow",typFixed,2,typOffensive },
	{ "Fireblade",typFixed,4,typOffensive },
	{ "Ironhand",typVariable,0,typOffensive },
	{ "Multimissile",typVariable,0,typOffensive },
	{ "Speedart",typFixed,1,typOffensive },
	{ "Countermagic",typVariable,0,typDefensive },
	{ "Shimmer",typVariable,0,typDefensive },
	{ "Slow",typVariable,0,typDefensive },
	{ "Spiritscreen",typVariable,0,typDefensive },
	{ "Dispel Magic",typVariable,0,typDefensive },
	{ "Protection",typVariable,0,typDefensive },
	{ "Silence",typVariable,0,typDefensive },
	{ "Glue",typVariable,0,typDefensive },
	{ "Darkwall",typFixed,2,typDefensive },
	{ "Lightwall",typFixed,4,typDefensive },
	{ "Farsee",typVariable,0,typEnhancement },
	{ "Endurance",typVariable,0,typEnhancement },
	{ "Strength",typVariable,0,typEnhancement },
	{ "Coordination",typVariable,0,typEnhancement },
	{ "Glamour",typVariable,0,typEnhancement },
	{ "Mobility",typVariable,0,typEnhancement },
	{ "Vigor",typVariable,0,typEnhancement },
	{ "Fanaticism",typFixed,1,typEnhancement },
	{ "Second Sight",typFixed,3,typEnhancement },
	{ "Visibility",typFixed,2,typEnhancement },
	{ "Control (Species)",typFixed,1,typMisc },
	{ "Detect Enemies",typFixed,1,typMisc },
	{ "Detect Magic",typFixed,1,typMisc },
	{ "Detect (Substance)",typFixed,1,typMisc },
	{ "Extinguish",typVariable,0,typMisc },
	{ "Ignite",typFixed,1,typMisc },
	{ "Heal",typVariable,0,typMisc },
	{ "Light",typFixed,1,typMisc },
	{ "Mind Speech",typVariable,0,typMisc },
	{ "Repair",typVariable,0,typMisc },
	{ "Armoring Ritual",typVariable,0,typRitual },
	{ "Summoning Ritual",typVariable,0,typRitual },
	{ "Binding Ritual",typVariable,0,typRitual },
	{ "Magic Point Ritual",typVariable,0,typRitual },
	{ "Spell Matrix Ritual",typVariable,0,typRitual },
};
