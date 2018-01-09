//**********************************************************************
//	Data Structures
//**********************************************************************
struct tagRaceTable {
	char *	Name;
	char *	StrRoll;
	char *	ConRoll;
	char *	SizRoll;
	char *	IntRoll;
	char *	PowRoll;
	char *	DexRoll;
	char *	AppRoll;
};

//**********************************************************************
//	PC Class
//**********************************************************************
class cPC {
	private:
		int		Age;

		int		Str;
		int		Con;
		int		Siz;
		int		Int;
		int		Pow;
		int		Dex;
		int		App;
		struct tagRaceTable	Race;

		static int GetPrimary(int Value)
			{ return(Value - 10); }
		static int GetNegative(int Value)
			{ return(-(Value - 10)); }
		static int GetSecondary(int Value)
			{ return((Value - 9) / 2); }

	public:
		static cPC * Select(void);

		cPC(struct tagRaceTable RaceTable);

		char * GetName(void)
			{ return(Race.Name); }

		int GetAge(void)
			{ return(Age); }
		void SetAge(int aAge)
			{ Age = aAge; }

		int GetStr(void)
			{ return(Str); }
		int GetCon(void)
			{ return(Con); }
		int GetSiz(void)
			{ return(Siz); }
		int GetInt(void)
			{ return(Int); }
		int GetDex(void)
			{ return(Dex); }
		int GetPow(void)
			{ return(Pow); }
		int GetApp(void)
			{ return(App); }

		char * GetStrRoll(void)
			{ return(Race.StrRoll); }
		char * GetConRoll(void)
			{ return(Race.ConRoll); }
		char * GetSizRoll(void)
			{ return(Race.SizRoll); }
		char * GetIntRoll(void)
			{ return(Race.IntRoll); }
		char * GetDexRoll(void)
			{ return(Race.DexRoll); }
		char * GetPowRoll(void)
			{ return(Race.PowRoll); }
		char * GetAppRoll(void)
			{ return(Race.AppRoll); }

		void SetStr(int aStr)
			{ Str = aStr; }
		void SetCon(int aCon)
			{ Con = aCon; }
		void SetSiz(int aSiz)
			{ Siz = aSiz; }
		void SetInt(int aInt)
			{ Int = aInt; }
		void SetPow(int aPow)
			{ Pow = aPow; }
		void SetDex(int aDex)
			{ Dex = aDex; }
		void SetApp(int aApp)
			{ App = aApp; }

		void GetHitPoints(
			int &	Total,
			int &	Head,
			int &	Chest,
			int &	Arm,
			int &	Abs,
			int &	Leg);

		char * GetDamageModifier(void);

		int GetMagicPoints(void)
			{ return(GetPow()); }
		int GetFatiguePoints(void)
			{ return(GetStr() + GetCon()); }

		int	GetDexSRM(void);
		int	GetSizSRM(void);

		void GetSkillBonuses(
			int &	Agility,
			int &	Knowledge,
			int &	Manipulate,
			int &	Stealth,
			int &	Communicate,
			int &	Magical,
			int &	Perception,
			int &	Attack,
			int &	Parry);

		void RollStats(void);
		void RollAvgStats(void);
		void Output();
		void Print(FILE * fptr);
};
