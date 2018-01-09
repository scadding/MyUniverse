/**************************************************************************/
/*                                                                        */
/*   Program Name : full_upp.c                                            */
/*                                                                        */
/*   Description : Expands the upp for a planetary system to a full       */
/*                 description.                                           */
/*                                                                        */
/*                 Compiled on TurboC 2.0 using:                          */
/*                                                                        */
/*                       tcc -mt -lt full_upp.c                           */
/*                                                                        */
/*------------------------------------------------------------------------*/
/* Paul J. Netherwood  Kingston Polytechnic               | 28th Feb 1990 */
/*                                                        |               */
/*------------------------------------------------------------------------*/
/* Modification history:                                                  */
/*------------------------------------------------------------------------*/
/* none                                                   |               */
/**************************************************************************/

#include <stdio.h>
#include <ctype.h>

void
main(int argc, char *argv[])
{
int	size, atmos, hydro, pop, gov;
int cloudiness;

     if(argc == 1)
     {
           printf("Usage: full_upp <starb><size><atmos><hydro><pop><govt><law><tech>\n");
           return;
     }

     printf("Planetary details:\n");

	/* starport */
     printf("Starport class %c, ",toupper(argv[1][0]));

     switch(tolower(argv[1][0]))
     {
           case 'a' : printf("Excellent quality installation\n");
                      printf("Refined fuel available. Annual maintenance overhaul available. Shipyard\n");
                      printf("capabable of constructing starships and non-starships present.\n");
                      break;

           case 'b' : printf("Good quality installation\n");
                      printf("Refined fuel available. Annual maintenance overhaul available. Shipyard\n");
                      printf("capabable of constructing non-starships present.\n");
                      break;

           case 'c' : printf("Routine quality installation\n");
                      printf("Only unrefined fuel available. Reasonable repair facilities present.\n");
                      break;

           case 'd' : printf("Poor quality installation\n");
                      printf("Only unrefined fuel available. No repair facilities present.\n");
                      break;

           case 'e' : printf("Frontier installation\n");
                      printf("Essentially a marked spot of bedrock with no fuel, facilities, or\n");
                      printf("bases present.\n");
                      break;

           case 'f' : printf("Spaceport:Good quality installation\n");
                      printf("Only unrefined fuel available. Minor repair facilities.\n");
                      printf("Spaceport used mainly for inter-system travel.\n");
                      break;
                      
           case 'g' : printf("Spaceport:Poor quality installation\n");
                      printf("Only unrefined fuel available. No repair facilities.\n");
                      printf("Spaceport used mainly for inter-system travel.\n");
                      break;

           case 'h' : printf("Spaceport:Primitive Installation; no facilities.\n");
                      printf("Spaceport used mainly for inter-system travel.\n");
                      break;

           case 'y' : printf("No spaceport.\n");
                      break;

           case 'x' : printf("No starport. No provision is made for any ship landings.\n");
                      break;

           default:   printf("Not a listed starport type\n");
     }

	/* size */
	switch(tolower(argv[1][1]))
	{
           case '0' : printf("Asteroid/Planetoid Belt.");
                      break;

           case 'r' : printf("Ring (around a world)\n");
                      break;

           case 's' : printf("Small World (200km) Surface gravity of .024 of a G\n");
                      break;

           case '1' : printf("Small world. Surface gravity of .122 of a G \n");
                      break;

		case '2' : printf("Small world. Surface gravity of .24 of a G \n");
                      break;

		case '3' : printf("Small world. Surface gravity of 37.7 of a G\n");
                      break;

		case '4' : printf("Small world. Surface gravity of .5 of a G \n");
                      break;

		case '5' : printf("Medium sized world. Surface gravity of .625 of a G \n");
                      break;

		case '6' : printf("Medium sized world. Surface gravity of .84 of a G \n");
                      break;

		case '7' : printf("Medium sized world. Surface gravity of .875 of a G \n");
                      break;


		case '8' : printf("Medium sized world. Surface gravity 1 G \n");
                      break;

		case '9' : printf("Large world. Surface gravity of 1.12 G's \n");
                      break;

           case 'a' : printf("Large world. Surface gravity of 1.25 G's \n");
                      break;

           default :  printf("Non standard world size.");
	}

     if(tolower(argv[1][1]) == 'a')
           size = 10;
     else
           if(tolower(argv[1][1]) == 'r' || tolower(argv[1][1]) == 's')
                 size = 0;
           else
                 size = (int)argv[1][1] - 48;

     if(size > 0 && size <= 10)
           printf("World diameter is %d miles (%d km).\n",(size*1000),(size*1600));

	/* atmos */
     if((int)argv[1][2] >= 141)
           atmos = (int)argv[1][2] - 131;
     else
           atmos = (int)argv[1][2] - 48;
	switch(tolower(argv[1][2]))
	{
           case '0' : printf("No atmosphere. Vacc suit needed.\n");
                      break;

           case '1' : printf("Trace atmosphere. Vacc suit needed.\n");
                      break;

           case '2' : printf("Very thin, tainted atmosphere. Combination mask needed.\n");
                      break;

           case '3' : printf("Very thin atmosphere. Compressor mask needed.\n");
                      break;

           case '4' : printf("Thin, tainted atmosphere. Filter mask needed.\n");
                      break;

           case '5' : printf("Thin atmosphere.\n");
                      break;

           case '6' : printf("Standard atmosphere.\n");
                      break;

           case '7' : printf("Standard, tainted atmosphere. Filter mask needed.\n");
                      break;

           case '8' : printf("Dense atmosphere.\n");
                      break;

           case '9' : printf("Dense, tainted atmosphere. Filter mask needed.\n");
                      break;

           case 'a' : printf("Exotic atmosphere. Oxygen tanks needed.\n");
                      break;

           case 'b' : printf("Corrosive atmosphere. Vacc suit or protective suit needed.\n");
                      break;

           case 'c' : printf("Insidious atmosphere. Vacc suit or protective suit needed.\n");
                      printf("Although it will only last for 2 to 12 hours\n");
                      break;

           case 'd' : printf("Dense, High atmosphere. The air pressure at sea (or lower)\n");
                      printf("levels is too great to support life, but at higher altitudes,\n");
                      printf("the atmosphere is breathable.\n");
                      break;

           case 'e' : printf("The worlds surface is ellipsoidal, not spherical in shape.\n");
                      printf("Because the atmosphere remains spherical, surface atmospheric\n");
                      printf("pressure ranges from very high at the middle to very low at the\n");
                      printf("ends. Breathable bands may exist at some point within the range\n");
                      printf("of pressure.\n");
                      break;

           case 'f' : printf("Thin, Low atmosphere. The world is a large, massive one with a thin\n");
                      printf("atmosphere which settles in the lowest levels of the terrain. As a\n");
                      printf("result, the atmosphere is unbreathable at most altitudes, but is\n");
                      printf("breathable at very low atltitudes (as in depressions or deep valleys).\n");


           default: printf("Non standard atmosphere classification.\n");
	}

	/* hydro */
	switch(tolower(argv[1][3]))
	{
           case '0' : hydro = 0;
                      printf("No free standing water.\n");
                      break;

           case '1' : 
		case '2' :
		case '3' :
		case '4' :
		case '5' :
		case '6' :
		case '7' :
		case '8' :
		case '9' : if(tolower(argv[1][3]) == 'a')
                             hydro = 10;
                      else
                             hydro = (int)argv[1][3] - 48;
                      printf("%-2d percent water. ",hydro * 10);
                      switch(hydro)
                      {
                             case 0 : cloudiness = 0;
                                      break;

                             case 1 : cloudiness = 0;
                                      break;

                             case 2 : cloudiness = 10;
                                      break;

                             case 3 : cloudiness = 10;
                                      break;

                             case 4 : cloudiness = 20;
                                      break;

                             case 5 : cloudiness = 30;
                                      break;

                             case 6 : cloudiness = 40;
                                      break;

                             case 7 : cloudiness = 50;
                                      break;

                             case 8 : cloudiness = 60;
                                      break;

                             case 9 : cloudiness = 70;
                                      break;

                             case 10: cloudiness = 70;
                                      break;
                      }
                      if(atmos >= 10)
                             if(cloudiness == 70)
                                   cloudiness = 100;
                             else 
                                   cloudiness += 40;
                      else
                             if(atmos < 3 && cloudiness > 20)
                                   cloudiness = 20;
                             else
                                   if(atmos == 14)
                                         cloudiness /= 2;
                      printf("Cloud cover is %d percent\n",cloudiness);
                      break;

           case 'a' : printf("No land masses.\n");
                      break;

           default : printf("Non standard hydrographics classification.\n");
	}

	/* pop */
     pop = (int)argv[1][4] - 48;
	switch(tolower(argv[1][4]))
	{
           case '0' : printf("No inhabitants.\n");
                      break;

           case '1' : printf("Tens of inhabitants.\n");
                      break;

		case '2' : printf("Hundreds of inhabitants.\n");
                      break;

		case '3' : printf("Thousands of inhabitants.\n");
                      break;

		case '4' : printf("Tens of thousands of inhabitants.\n");
                      break;

		case '5' : printf("Hundreds of thousands of inhabitants.\n");
                      break;

		case '6' : printf("Millions of inhabitants.\n");
                      break;

		case '7' : printf("Tens of millions of inhabitants.\n");
                      break;

		case '8' : printf("Hundreds of millions of inhabitants.\n");
                      break;

		case '9' : printf("Billions of inhabitants.\n");
                      break;

           case 'a' : pop = 10;
                      printf("Tens of billions of inhabitants.\n");
                      break;

           default : printf("Non standard population classification.\n");
	}

     /* Government */
     gov = (int)argv[1][5] - 48;
     switch(tolower(argv[1][5]))
     {
           case '0' : printf("No government structure. In many cases, family bonds predominate.\n");
                      break;

           case '1' : printf("Company/Corporation. Government by a company managerial elite; citizens\n");
                      printf("are company employees.\n");
                      break;

		case '2' : printf("Participating Democracy. Government by advice and consent of the citizen.\n");
                      break;

		case '3' : printf("Self-Perpetuating Oligarchy. Government by a restricted minority, with\n");
                      printf("little or no input from the masses.\n");
                      break; 

		case '4' : printf("Representative Democracy. Government by elected representatives.\n");
                      break;

		case '5' : printf("Feudal Technocracy. Government by specific individuals for those who\n");
                      printf("agree to be ruled. Relationships are based on peformance of technical\n");
                      printf("activities which are mutually beneficial.\n");
                      break;

		case '6' : printf("Captive Government. Government by a leadership answerable to an outside\n");
                      printf("group; a colony or conqured area.\n");
                      break;

		case '7' : printf("Balcanization. No central ruling authority exists; rival governments compete\n");
                      printf("for control.\n");
                      break;

		case '8' : printf("Civil Service Bureaucracy. Government by agencies employing individuals\n");
                      printf("selected for their expertise.\n");
                      break;

		case '9' : printf("Impersonal Bureaucracy. Government by agencies which are insulated from\n");
                      printf("the governed.\n");
                      break;

           case 'a' : printf("Charismatic Dictator. Government by a single leader enjoying the confidence\n");
                      printf("of the citizens.\n");
                      break;

           case 'b' : printf("Non-Charismatic Dictator. A previous charismatic dictator has been replaced\n");
                      printf("by a leader through normal channels.\n");
                      break;

           case 'c' : printf("Charismatic Oligarchy. Government by a selected group, organisation, or class\n");
                      printf("enjoying the overwhelming confidence of the citizenry.\n");
                      break;

           case 'd' : printf("Religious Dictatorship. Government by a religious organisation without regard\n");
                      printf("to the needs of the citizenry.\n");
                      break;

           default : printf("Non standard government classification.\n");

     }

	/* law */
     printf("Law Level %c: ",toupper(argv[1][6]));
	switch(tolower(argv[1][6]))
	{
           case '0' : printf("No weapon prohibitions.\n");
                      break;

           case '1' : printf("Body pistols undetectable by standard detectors, \n");
                      printf("expolosives (bombs, grenades), and poison gas prohibited.\n");
                      break;

		case '2' : printf("Portable energy weapons (laser carbine, laser rifle)\n");
                      printf("prohibited. Ships gunnery not affected.\n");
                      break;

		case '3' : printf("Weapons of a strict millitary nature (machine guns,\n");
                      printf("automatic rifles) prohibited.\n");
                      break; 

		case '4' : printf("Light assault weapons (sub-machine guns) prohibited.\n");
                      break;

		case '5' : printf("Personal concealable firearms (such as pistols and\n");
                      printf("revolvers) prohibited.\n");
                      break;

		case '6' : printf("Most firearms (all except shotguns) prohibited. The carrying of\n");
                      printf("any type of weapon openly is discouraged.\n");
                      break;

		case '7' : printf("Shotguns are prohibited.\n");
                      break;

		case '8' : printf("Long bladed weapons (all but daggers) are controlled, and open\n");
                      printf("possesion is prohibited.\n");
                      break;

		case '9' : printf("Possession of any weapon outside one's residence is prohibited.\n");
                      break;

           default  : printf("Possession of any weapon is prohibited.\n");
                      break;
	}

	/* tech */
     printf("Tech Level %c: ",toupper(argv[1][7]));
	switch(tolower(argv[1][7]))
	{
           case '0' : printf("Stone Age. Primitive.\n");
                      break;

           case '1' : printf("Bronze Age to Middle Ages.\n");
                      break;

		case '2' : printf("circa 1400 to 1700\n");
                      break;

		case '3' : printf("circa 1700 to 1860\n");
                      break; 

		case '4' : printf("circa 1860 to 1900\n");
                      break;

		case '5' : printf("circa 1900 to 1939\n");
                      break;

		case '6' : printf("circa 1940 to 1969\n");
                      break;

		case '7' : printf("circa 1970 to 1979\n");
                      break;

		case '8' : printf("circa 1980 to 1989\n");
                      break;

		case '9' : printf("circa 1990 to 2000\n");
                      break;

           case 'a' : printf("Interstellar community\n");
                      break;

           case 'b' : printf("Average Imperial tech level\n");    
                      break;

           case 'c' : printf("Average Imperial tech level\n");
                      break;

           case 'd' : printf("Above average Imperial tech level\n");
                      break;

           case 'e' : printf("Above average Imperial tech level\n");
                      break;

           case 'f' : printf("Technical maximum Imperial\n");
                      break;

           case 'g' : printf("Occasional non-Imperial\n");
                      break;

           default : printf("Non standard tech level classification.\n");
	}

     /* Trade classifications */
     printf("Trade Classifications: ");
     if(atmos >= 4 && atmos <= 9 && hydro >= 4 && hydro <= 8
     && pop >= 5 && pop <= 7)
           printf("Agricultural. ");

     if(atmos < 3 && hydro < 3 && pop >= 6)
           printf("Non-Agricultural. ");

     switch(atmos)
     {
           case 0 :
           case 1 :
           case 2 :
           case 4 :
           case 7 :
           case 9 : if(pop >= 9)
                         printf("Industrial. ");
     }

     if(pop < 6)
           printf("Non-Industrial. ");

     if( (atmos == 6 || atmos == 8) && pop >= 6 && pop <= 8 
     && gov >= 4 && gov <= 9)
           printf("Rich. ");

     if(atmos >= 2 && atmos <= 5 && hydro < 3)
           printf("Poor. ");

     if(hydro == 10)
           printf("Water World. ");

     if(hydro == 0 && atmos >= 2)
           printf("Desert World. ");

     if(atmos == 0)
           printf("Vacuum World. ");

     if(tolower(argv[1][1]) == '0')
           printf("Asteroid Belt. ");

     if( (atmos == 0 || atmos == 1) && hydro >= 1)
           printf("Ice Capped. ");
}
