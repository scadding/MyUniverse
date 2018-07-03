//**********************************************************************
//	Include Files
//**********************************************************************
#include "skills.h"

//**********************************************************************
//	Background Class
//**********************************************************************
class cBackgrnd : 
    public cSkills {
	private:
		int CheckSkillLevel(int Score, int Bonus);
	public:
		static cBackgrnd * Select(void);

		cBackgrnd(struct tagSkillsTable SkillsTable) :
				cSkills(SkillsTable)
				{}
		void SetExp(int	aExp)
			{ cSkills::SetExp(aExp); }

		void Output(cPC & PC);
		void Print(cPC & PC,FILE * fptr);
};
