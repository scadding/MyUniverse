
struct tagSkillsTable {
	char *	Name;
	int	Boat; 		int	Climb; 			int	Dodge;
	int	Jump; 		int	Ride; 			int	Swim;
	int	Throw; 		int	FastTalk; 		int	Orate;
	int	Sing; 		int	SpeakLang; 		int	Animal;
	int	Craft; 		int	Evaluate; 		int	FirstAid;
	int	Human; 		int	MartialArt; 	int	Mineral;
	int	Plant; 		int	ReadLang; 		int	Ships;
	int	World; 		int	Conceal; 		int	Devise;
	int	Sleight; 	int	Instrument; 	int	Listen;
	int	Scan; 		int	Search; 		int	Track;
	int	Hide; 		int	Sneak; 			int	Ceremony;
	int	Summon; 	int	Enchant; 		int	Fist;
	int	Dagger; 	int	Wpn1Hand; 		int	Wpn2Hand;
	int	Shield;		int	Missile;
	char *	Equipment;
	int		SpellPts;
	char *	Spells;
};

//**********************************************************************
//	Skills Class
//**********************************************************************
class cSkills {
private:
	int Exp;
	struct tagSkillsTable Bonus;
public:
	cSkills(struct tagSkillsTable Table)
		{ Exp = 0; Bonus = Table; }
	void SetExp(int aExp)
		{ Exp = aExp; }
	char * GetName(void)
		{ return(Bonus.Name); }
	int GetBoat(void)
		{ return(Bonus.Boat * Exp + 5); }
	int GetClimb(void)
		{ return(Bonus.Climb * Exp + 40); }
	int GetDodge(void)
		{ return(Bonus.Dodge * Exp + 5); }
	int GetJump(void)
		{ return(Bonus.Jump * Exp + 25); }
	int GetRide(void)
		{ return(Bonus.Ride * Exp + 5); }
	int GetSwim(void)
		{ return(Bonus.Swim * Exp + 15); }
	int GetThrow(void)
		{ return(Bonus.Throw * Exp + 25); }
	int GetFastTalk(void)
		{ return(Bonus.FastTalk * Exp + 5); }
	int GetOrate(void)
		{ return(Bonus.Orate * Exp + 5); }
	int GetSing(void)
		{ return(Bonus.Sing * Exp + 5); }
	int GetSpeak(void)
		{ return(Bonus.SpeakLang* Exp + 0); }
	int GetAnimal(void)
		{ return(Bonus.Animal * Exp + 5); }
	int GetCraft(void)
		{ return(Bonus.Craft * Exp + 10); }
	int GetEvaluate(void)
		{ return(Bonus.Evaluate * Exp + 5); }
	int GetFirstAid(void)
		{ return(Bonus.FirstAid * Exp + 10); }
	int GetRace(void)
		{ return(Bonus.Human * Exp + 5); }
	int GetMartialArt(void)
		{ return(Bonus.MartialArt * Exp + 0); }
	int GetMineral(void)
		{ return(Bonus.Mineral * Exp + 5); }
	int GetPlant(void)
		{ return(Bonus.Plant * Exp + 5); }
	int GetRead(void)
		{ return(Bonus.ReadLang * Exp + 0); }
	int GetShips(void)
		{ return(Bonus.Ships * Exp + 0); }
	int GetWorld(void)
		{ return(Bonus.World * Exp + 5); }
	int GetConceal(void)
		{ return(Bonus.Conceal * Exp + 5); }
	int GetDevise(void)
		{ return(Bonus.Devise * Exp + 5); }
	int GetSleight(void)
		{ return(Bonus.Sleight * Exp + 5); }
	int GetInstrument(void)
		{ return(Bonus.Instrument * Exp + 0); }
	int GetListen(void)
		{ return(Bonus.Listen * Exp + 25); }
	int GetScan(void)
		{ return(Bonus.Scan * Exp + 25); }
	int GetSearch(void)
		{ return(Bonus.Search * Exp + 25); }
	int GetTrack(void)
		{ return(Bonus.Track * Exp + 5); }
	int GetHide(void)
		{ return(Bonus.Hide * Exp + 10); }
	int GetSneak(void)
		{ return(Bonus.Sneak * Exp + 10); }
	int GetCeremony(void)
		{ return(Bonus.Ceremony * Exp + 5); }
	int GetSummon(void)
		{ return(Bonus.Summon * Exp + 0); }
	int GetEnchant(void)
		{ return(Bonus.Enchant * Exp + 0); }
	int GetFist(void)
		{ return(Bonus.Fist * Exp + 25); }
	int GetDagger(void)
		{ return(Bonus.Dagger * Exp + 15); }
	int GetWpn1Hand(void)
		{ return(Bonus.Wpn1Hand * Exp + 10); }
	int GetWpn2Hand(void)
		{ return(Bonus.Wpn2Hand * Exp + 5); }
	int GetShield(void)
		{ return(Bonus.Shield * Exp + 15); }
	int GetMissile(void)
		{ return(Bonus.Missile * Exp + 5); }
};

