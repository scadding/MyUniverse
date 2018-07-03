/*

Cosmos # 14;  This program automates cosmos #11, 12 and 13.  Compile
"cc -o sgen sgen.c -lm"  There are no bugs.  And if you believe that
I have a farm to sell you in the sky.  Suggestions and errors will be
greatly appreciated.

-John R.
*/

#include <stdio.h>

#define GRAN 10

#define NOCOMSS 20
#define JUMP 0
#define LIB JUMP+1
#define CBT LIB+1
#define SKL CBT+1
#define FWD SKL+1
#define COMM FWD+1
#define PER05 0
#define PER10 PER05+1
#define PER12 PER10+1
#define PER15 PER12+1
#define PER20 PER15+1
#define PER23 PER20+1
#define PER25 PER23+1
#define PER30 PER25+1
#define PER35 PER30+1
#define PER40 PER35+1
#define PER45 PER40+1
#define PER50 PER45+1
#define PER90 PER50+1
#define PER0 PER90+1

#define PRINT 1
#define NOPRINT 0
#define SLEN 256
#define PATHSIZE 256

double log();
FILE *fopen();

int getcn();
int tech_level();
int emass();
int num_computers();
int int_computers();
int computers_jobs();
int num_eweapons();
int screens();
int life_support();
int cryogenics();
int floor_s_cubed();
int hull_mass();
int mad_mass();
int cong_mass();
int ship_dens();
int jumpdrive();
int psionics();
int report();
int explosion();
int grav_drive();
int free_eunits();

int (*emass_action[])()= {
    life_support,
    explosion,
    free_eunits,
    0
};

int (*tl_action[])()= {
    emass,
    num_eweapons,
    screens,
    life_support,
    cryogenics,
    floor_s_cubed,
    hull_mass,
    cong_mass,
    jumpdrive,
    explosion,
    grav_drive,
    free_eunits,
    0
};
int (*eweap_action[])()= {
    jumpdrive,
    explosion,
    grav_drive,
    0
};

int (*init_action[])()= {
    tech_level,
    emass,
    num_computers,
    int_computers,
    computers_jobs,
    num_eweapons,
    screens,
    life_support,
    cryogenics,
    floor_s_cubed,
    hull_mass,
    mad_mass,
    cong_mass,
    ship_dens,
    jumpdrive,
    explosion,
    grav_drive,
    free_eunits,
    psionics,
    0
};

struct cyber {
    float comps;
    int compt;
    int psi;
    int sec_nerv;
    int pri_nerv;
    int pss;
    int psr;
};

struct ship_archi {
    int nc;
    struct cyber cyber[NOCOMSS];
    float tl;
    float emass;
    float intel;
    float eunits;
    float ocvbase;
    float ocvmass;
    float now;
    float odvenbase;
    float dcvbase;
    float dscbase;
    float dscmass;
    float scsum;
    float isyseunits;
    float isysmass;
    float cryeunits;
    float cryscsum;
    float flsq3;
    float flmass;
    float bsm;
    float odvenmass;
    float dcvmass;
    float crymass;
    float humass;
    float hpips;
    float hudv;
    float madmass;
    float cgunits;
    float cgmass;
    float cgrange;
    float madgbase;
    float tsm;
    float jumass;
    float jubase;
    float jubase2;
    float jubase5;
    float jutime;
    float dens;
    float explr;
    float feunits;
    float scbase;
} ship_archi;

float logtl;
float logtlm6;
float logtlm7;
float logtlm8;
float logtlm9;
float logtlm10;
float logtlm14;
float logtlm18;

main(argc, argv)
int argc;
char **argv;
{
    for(/* ever*/ ;;) {
        switch(get_menu()) {
        case 0:
            exit(0);
        case 1:
            s_from_scratch();
            break;
        case 2:
            report();
            break;
        case 3:
            freport();
            break;
        case 4:
            c_emass();
            break;
        case 5:
            c_tl();
            break;
        case 6:
            c_eweap();
            break;
        case 7:
            c_num_comp();
            break;
        case 8:
            c_int_comp();
            break;
        case 9:
            c_job_comp();
            break;
        case 10:
            c_life_support();
            break;
        case 11:
            c_cryo();
            break;
        case 12:
            c_floors3();
            break;
        case 13:
            c_hull_mass();
            break;
        case 14:
            c_cong_mass();
            break;
        case 15:
            c_ship_dens();
            break;
        default:
            fprintf(stderr, "No nO no \n");
        }
    }
}

get_menu()
{
    int answ;

    fprintf(stdout, "exit -> 0\nstart_over -> 1\nreport -> 2\n");
    fprintf(stdout, "cp to file -> 3\nchange emass -> 4\n");
    fprintf(stdout, "change tech level -> 5\nchange no energy weapons -> 6\n");
    fprintf(stdout, "change number of computer systems -> 7\n");
    fprintf(stdout, "change intelligence of computer systems -> 8\n");
    fprintf(stdout, "change jobs of computer systems -> 9\n");
    fprintf(stdout, "change life support systems -> 10\n");
    fprintf(stdout, "change cryogenics -> 11\n");
    fprintf(stdout, "change cubic meters of ship floor -> 12\n");
    fprintf(stdout, "change hull mass -> 13\nchange contra grav mass -> 14\n");
    fprintf(stdout, "change ship density -> 15\n");
    scanf("%d", &answ);
    return(answ);
}

s_from_scratch()
{
    register int	(**ponent_ptr)();

    for (ponent_ptr= &init_action[0]; *ponent_ptr; ponent_ptr++)
        (**ponent_ptr)(PRINT);
}

c_emass()
{
    register int	(**ponent_ptr)();

    emass(PRINT);
    for (ponent_ptr= &emass_action[0]; *ponent_ptr; ponent_ptr++)
        (**ponent_ptr)(NOPRINT);
}

c_tl()
{
    register int	(**ponent_ptr)();

    tech_level(PRINT);
    for (ponent_ptr= &tl_action[0]; *ponent_ptr; ponent_ptr++)
        (**ponent_ptr)(NOPRINT);
}

c_eweap()
{
    register int	(**ponent_ptr)();

    num_eweapons(PRINT);
    for (ponent_ptr= &eweap_action[0]; *ponent_ptr; ponent_ptr++)
        (**ponent_ptr)(NOPRINT);
}

c_num_comp()
{

    num_computers(PRINT);
    int_computers(PRINT);
    computers_jobs(PRINT);
    num_eweapons(NOPRINT);

}

c_int_comp()
{

    int_computers(PRINT);
    computers_jobs(PRINT);
    psionics(NOPRINT);
    num_eweapons(NOPRINT);
}

c_job_comp()
{
    computers_jobs(PRINT);
    num_eweapons(NOPRINT);
}

c_life_support()
{
    life_support(PRINT);
    jumpdrive(NOPRINT);
    free_eunits(NOPRINT);
}

c_cryo()
{
    cryogenics(PRINT);
    jumpdrive(NOPRINT);
    free_eunits(NOPRINT);
}

c_floors3()
{
    floor_s_cubed(PRINT);
    jumpdrive(NOPRINT);
}

c_hull_mass()
{
    hull_mass(PRINT);
    jumpdrive(NOPRINT);
}

c_cong_mass()
{
    cong_mass(PRINT);
    jumpdrive(NOPRINT);
}

c_ship_dens()
{
    ship_dens(PRINT);
    explosion(NOPRINT);
}

tech_level(p)
int p;
{
    if(p) {
        printf("Enter TL <tech level> \n");
        scanf("%f", &ship_archi.tl);
    }

    logtlm7 = 0.0;
    if(ship_archi.tl <= 6.0)
        fprintf(stderr, "no space ships below tech level 7 \n");
    logtl = log(ship_archi.tl);
    if(ship_archi.tl > 6.0)
        logtlm6 = log(ship_archi.tl - 6.0);
    if(ship_archi.tl > 7.0)
        logtlm7 = log(ship_archi.tl - 7.0);
    if(ship_archi.tl > 8.0)
        logtlm8 = log(ship_archi.tl - 8.0);
    if(ship_archi.tl > 9.0)
        logtlm9 = log(ship_archi.tl - 9.0);
    if(ship_archi.tl > 10.0)
        logtlm10 = log(ship_archi.tl - 10.0);
    if(ship_archi.tl > 14.0)
        logtlm14 = log(ship_archi.tl - 14.0);
    if(ship_archi.tl > 18.0)
        logtlm18 = log(ship_archi.tl - 18.0);
    ship_archi.ocvmass = logtl * 7 / ship_archi.tl;
    ship_archi.dcvmass = logtlm7;
    return;
}

emass(p)
int p;
{
    ship_archi.eunits = 0.0;
    if(ship_archi.tl <= 8.0) {
        fprintf(stderr, "no high energy systems below tech level 9 \n");
        return;
    }
    if(p) {
        printf("Enter Emass <energy mass> \n");
        scanf("%f",  &ship_archi.emass);
    }
    ship_archi.eunits = logtl * ship_archi.emass;
}

num_computers(p)
int p;
{
    if(ship_archi.tl < 7.0) {
        fprintf(stderr, "no computer systems below tech level 7 \n");
        return;
    }
    printf("Enter number of computer systems \n");
    scanf("%d", &ship_archi.nc);
    while (ship_archi.nc > NOCOMSS) {
        printf("too many \n");
        printf("Enter number of computer sytems \n");
        scanf("%d", &ship_archi.nc);
    }
}

int_computers(p)
int p;
{
    float intel;
    int i;

    for (i = 0; i < ship_archi.nc; i++) {
        printf("Enter INT <intelligence> of computer \n");
        scanf("%f", &intel);
        ship_archi.cyber[i].comps = intel;
    }
}

computers_jobs(p)
int p;
{
    printf("computer no for jump ctl? \n");
    ship_archi.cyber[JUMP].compt = getcn();
    printf("computer no for library ctl? \n");
    ship_archi.cyber[LIB].compt = getcn();
    printf("computer no for offense/defense? \n");
    ship_archi.cyber[CBT].compt = getcn();
    printf("computer no for skill simulation? \n");
    ship_archi.cyber[SKL].compt = getcn();
    printf("computer no for observation? \n");
    ship_archi.cyber[FWD].compt = getcn();
    printf("computer no for communications? \n");
    ship_archi.cyber[COMM].compt = getcn();

    ship_archi.scbase = ship_archi.cyber[ship_archi.cyber[FWD].compt].comps *
                        logtlm7;
}

int getcn()
{
    int i;

    for(;; /* ever */) {
        scanf("%d", &i);
        if (i <= ship_archi.nc && i > 0)
            return(i - 1);
        printf("that one isn't there! \n");
    }
}

num_eweapons(p)
int p;
{
    if (ship_archi.tl <= 7.0) {
        fprintf(stderr, "no energy weapons below tech level 8");
        return;
    }
    ship_archi.ocvmass = logtl * 7 / ship_archi.tl;
    ship_archi.ocvbase = ship_archi.cyber[ship_archi.cyber[CBT].compt].comps * logtlm7;
    if(p) {
        printf("number of energy weapon systems? \n");
        scanf("%f", &ship_archi.now);
    }
    ship_archi.odvenbase = ship_archi.now * logtlm8;
    ship_archi.odvenmass = ship_archi.now / logtl;
    ship_archi.dcvbase = ship_archi.cyber[ship_archi.cyber[CBT].compt].comps * logtlm7;
}

screens(p)
int p;
{

    if(ship_archi.tl <= 10.0) {
        fprintf(stderr, "secondary energy screens do no exist before tech level");
        fprintf(stderr, " 11 \n");
        return;
    }
    ship_archi.dscbase = logtlm10 * logtl;
    ship_archi.dscmass = logtlm10 * 10.0 / ship_archi.tl;
}

life_support(p)
int p;
{
    if(ship_archi.tl <= 6.0) {
        fprintf(stderr, "life support systems do not exist before tech level");
        fprintf(stderr, " 7 \n");
        return;
    }
    if(p) {
        printf("total lifeform size supported <6.41 = large human> \n");
        scanf("%f", &ship_archi.scsum);
    }
    ship_archi.isyseunits = ship_archi.scsum / logtl;
    ship_archi.isysmass = ship_archi.isyseunits;
}

cryogenics(p)
int p;
{
    if(ship_archi.tl <= 8.0) {
        fprintf(stderr, "cryogenic systems do not exist before tech level");
        fprintf(stderr, " 9 \n");
        return;
    }
    if(p) {
        printf("total lifeform size supported by cryogenics \n");
        scanf("%f", &ship_archi.cryscsum);
    }
    ship_archi.cryeunits = ship_archi.cryscsum / logtl * logtl;
    ship_archi.crymass = ship_archi.cryeunits;
}

floor_s_cubed(p)
int p;
{
    if(ship_archi.tl <= 6.0) {
        fprintf(stderr, "spaceships do not exist before tech level");
        fprintf(stderr, " 7 \n");
        return;
    }
    if(p) {
        printf("enter floor space in cubic meters \n");
        scanf("%f", &ship_archi.flsq3);
    }
    ship_archi.flmass = ship_archi.flsq3 / (27 * logtl);
}

hull_mass(p)
int p;
{
    if(ship_archi.tl <= 6.0) {
        fprintf(stderr, "spaceships do not exist before tech level");
        fprintf(stderr, " 7 \n");
        return;
    }
    if(p) {
        printf("enter hull mass in metric tons \n");
        scanf("%f", &ship_archi.humass);
    }
    ship_archi.hudv = logtlm7 * ship_archi.humass / 100.0;
    ship_archi.hpips = ship_archi.hudv * 36.0;
}

mad_mass(p)
int p;
{
    if(ship_archi.tl <= 6.0) {
        fprintf(stderr, "spaceships do not exist before tech level");
        fprintf(stderr, " 7 \n");
        return;
    }
    printf("enter maneuver drive mass \n");
    scanf("%f", &ship_archi.madmass);
}

cong_mass(p)
int p;
{
    if (ship_archi.tl <= 9.0) {
        fprintf(stderr, "contra-gravity does not exist before tech level");
        fprintf(stderr, "10 \n");
        return;
    }
    if(p) {
        printf("enter contra-gravity mass \n");
        scanf("%f", &ship_archi.cgmass);
    }
    if(ship_archi.tl < 15.0) {
        ship_archi.cgunits = ship_archi.cgmass * logtlm9 * logtl;
        ship_archi.cgrange = ship_archi.cgmass;
    } else {
        if(ship_archi.tl < 19.0) {
            ship_archi.cgunits = ship_archi.cgmass * logtlm14 * logtlm14 * logtl;
            ship_archi.cgrange = ship_archi.cgmass;
        } else {
            ship_archi.cgunits = ship_archi.cgmass * logtlm18 * logtlm18 * logtl
                                 * logtl;
            ship_archi.cgrange = ship_archi.cgmass * logtl *logtl;
        }
    }
}

ship_dens(p)
int p;
{
    if(ship_archi.tl <= 6.0) {
        fprintf(stderr, "spaceships do not exist before tech level");
        fprintf(stderr, " 7 \n");
        return;
    }
    printf("enter density of ship as percentage \n");
    scanf("%f", &ship_archi.dens);
    ship_archi.dens /= 100.0;
}

jumpdrive(p)
int p;
{
    if(ship_archi.tl <= 9.0) {
        fprintf(stderr, "jumpdrive does not exist before tech level");
        fprintf(stderr, " 10 \n");
        return;
    }
    ship_archi.bsm = ship_archi.flmass + ship_archi.isysmass + ship_archi.dscmass
                     + ship_archi.ocvmass + ship_archi.emass + ship_archi.humass
                     + ship_archi.dcvmass + ship_archi.odvenmass +
                     ship_archi.crymass + ship_archi.madmass + ship_archi.cgmass;

    if (ship_archi.tl > 9.0 && ship_archi.tl <= 14.0) {
        ship_archi.jubase = logtl * logtlm9 / ship_archi.bsm;
        ship_archi.jumass = ship_archi.jubase * 100.0 / logtl;
        ship_archi.tsm = ship_archi.bsm + ship_archi.jumass;
        ship_archi.jubase = logtl * logtlm9 / ship_archi.tsm;
        ship_archi.jubase2 = logtl * logtlm9 / (ship_archi.tsm * 2);
        ship_archi.jubase5 = logtl * logtlm9 / (ship_archi.tsm * 5);
    } else {
        if (ship_archi.tl > 18.0) {
            ship_archi.jubase = logtl * logtlm18 / ship_archi.bsm;
            ship_archi.jumass = ship_archi.jubase / logtl;
            ship_archi.tsm = ship_archi.bsm + ship_archi.jumass;
            ship_archi.jubase = logtl * logtlm18 / ship_archi.tsm;
            ship_archi.jubase2 = logtl * logtlm18 / (ship_archi.tsm * 2);
            ship_archi.jubase5 = logtl * logtlm18 / (ship_archi.tsm * 5);
        } else {
            ship_archi.jubase = logtl * logtlm14 / ship_archi.bsm;
            ship_archi.jumass = ship_archi.jubase / logtl;
            ship_archi.tsm = ship_archi.bsm + ship_archi.jumass;
            ship_archi.jubase = logtl * logtlm14 / ship_archi.tsm;
            ship_archi.jubase2 = logtl * logtlm14 / (ship_archi.tsm * 2);
            ship_archi.jubase5 = logtl * logtlm14 / (ship_archi.tsm * 5);
        }
    }
}

explosion(int p)
{
    if(ship_archi.tl <= 5.0) {
        fprintf(stderr, "missiles do not exist before tech level");
        fprintf(stderr, " 6 \n");
        return;
    }
    ship_archi.explr = logtlm6 * ship_archi.eunits * ship_archi.tsm *
                       ship_archi.dens;
}

grav_drive(int p)
{
    if(ship_archi.tl <= 8.0) {
        fprintf(stderr, "gravatic drives do not exist before tech level");
        fprintf(stderr, " 9 \n");
        return;
    }
    ship_archi.madgbase = logtlm8 * ship_archi.madmass;
}

free_eunits(int p)
{
    ship_archi.feunits = ship_archi.eunits - ship_archi.isyseunits -
                         ship_archi.cryeunits;
}

psionics(p)
int p;
{
    int i;

    for (i = 0; i < ship_archi.nc; i++) {
        if(p) {
            printf("psionic base computer no %d:", i);
            scanf("%d", &ship_archi.cyber[i].psi);
            printf("/ncomputer secondary nerve rating [DEX]:");
            scanf("%d", &ship_archi.cyber[i].sec_nerv);
            printf("/ncomputer primary nerve rating [STR]:");
            scanf("%d", &ship_archi.cyber[i].pri_nerv);
        }
        ship_archi.cyber[i].pss = ship_archi.cyber[i].psi * 2 + ship_archi.cyber[i].comps + ship_archi.cyber[i].pri_nerv;
        ship_archi.cyber[i].psr = ship_archi.cyber[i].psi * 2 + ship_archi.cyber[i].comps + ship_archi.cyber[i].sec_nerv;
    }
}

report()
{
    FILE *fdes;
    fdes = stdout;
    report_1(fdes);
}

freport()
{
    FILE *fdes;
    char fname[PATHSIZE];
    fprintf(stdout, "name of file : ");
    scanf("%s", fname);
    if((fdes = fopen(fname, "w")) < 0) {
        perror(fname);
        return;
    }

    report_1(fdes);
}

report_1(fdes)
FILE *fdes;
{
    static float fep[] = {0.05, 0.10, 0.12, 0.15, 0.20, 0.23, 0.25, 0.30, 0.35,
                          0.40, 0.45, 0.50, 0.90, -1.0
                         };
    static float fes[] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                          0.00, 0.00, 0.00, 0.00, 0.00
                         };

#define SCAL_PER0(x)                       \
        x, x * fes[PER05],                 \
	x * fes[PER10], x * fes[PER12],    \
	x * fes[PER15]

#define SCAL_PER20(x)                      \
        x * fes[PER20], x * fes[PER23],    \
	x * fes[PER25], x * fes[PER30],    \
	x * fes[PER35]

#define SCAL_PER40(x)                      \
        x * fes[PER40], x * fes[PER45],    \
	x * fes[PER50], x * fes[PER90]

#define ADD_PER0(x)                       \
        x, x + fes[PER05],                 \
	x + fes[PER10], x + fes[PER12],    \
	x + fes[PER15]

#define ADD_PER20(x)                      \
        x + fes[PER20], x + fes[PER23],    \
	x + fes[PER25], x + fes[PER30],    \
	x + fes[PER35]

#define ADD_PER40(x)                      \
        x + fes[PER40], x + fes[PER45],    \
	x + fes[PER50], x + fes[PER90]


#define PRF_PER0(x)                        \
        "x 00per %f x 5per %f x 10per %f\n x 12per %f x 15per %f \n",

#define PRF_PER20(x)                        \
	"x 20per %f x 23per %f x 25per %f\n x 30per %f x 35per %f \n",

#define PRF_PER40(x)                        \
	"x 40per %f x 45per %f x 50per %f\n x 90per %f \n",

#define CONF(a, ocv, odven, dcv, dsc, madg, cg, ju, sc, res) \
fprintf(fdes, "\n a \n");                                         \
fprintf(fdes, "OCV %d ODVen %d DCV %d DSC %d MADG %d CGunits %d", \
	(((int)(ship_archi.ocvbase + fes[ocv])+ (GRAN / 2))/ GRAN ) * GRAN,   \
	(((int)(ship_archi.odvenbase + fes[odven])+ (GRAN / 2))/ GRAN ) * GRAN,   \
	(((int)(ship_archi.dcvbase + fes[dcv])+ (GRAN / 2))/ GRAN ) * GRAN,   \
	(((int)(ship_archi.dscbase * fes[dsc])+ (GRAN / 2))/ GRAN ) * GRAN,   \
	(((int)(ship_archi.madgbase / ship_archi.tsm * fes[madg])+ (GRAN / 2))/ GRAN ) * GRAN,   \
	(((int)(ship_archi.cgunits * fes[cg])+ (GRAN / 2))/ GRAN ) * GRAN);   \
fprintf(fdes, " JU %d SC %d FREE %d \n",                                       \
	(int)(ship_archi.jubase * fes[ju]),   \
	(((int)ship_archi.scbase + (int)fes[sc]+ (GRAN / 2))/ GRAN ) * GRAN,   \
	(((int)fes[res])/ GRAN + (GRAN / 2)) * GRAN);     \



    int i;
    char fname[30];
    char str[SLEN];

    for (i = 0; fep[i] != -1.0; i++)
        fes[i] = ship_archi.feunits * fep[i];

    fprintf (fdes, "emass %f eunits %f free-eunits %f tech-level %f \n", ship_archi.emass, ship_archi.eunits,
             ship_archi.feunits, ship_archi.tl);
    fprintf (fdes, "\nint computer communications %f \nobserve/scanning %f \nskills %f \n"
             ,ship_archi.cyber[ship_archi.cyber[COMM].compt].comps, ship_archi.cyber[ship_archi.cyber[FWD].compt].comps, ship_archi.cyber[ship_archi.cyber[SKL].compt].comps);
    fprintf (fdes, "combat %f \nlibary %f \nhyperspace %f \n", ship_archi.cyber[ship_archi.cyber[CBT].compt].comps,
             ship_archi.cyber[ship_archi.cyber[LIB].compt].comps, ship_archi.cyber[ship_archi.cyber[JUMP].compt].comps);
    fprintf(fdes, "\n");
    for(i = 0; i < ship_archi.nc; i++) {
        fprintf(fdes, "comp sys: %d psi: %d 2nd nerv: %d 1st nerv: %d",
                i, ship_archi.cyber[i].psi, ship_archi.cyber[i].sec_nerv,
                ship_archi.cyber[i].pri_nerv);
        fprintf(fdes, " psr: %d pss: %d \n",ship_archi.cyber[i].psr,
                ship_archi.cyber[i].pss);
    }
    fprintf(fdes, "\n");

    fprintf(fdes, "offensive damage mass %f \n", ship_archi.odvenmass);
    fprintf(fdes, "offensive damage base %f \n", ship_archi.odvenbase);
    fprintf(fdes, "\n");
    fprintf(fdes, PRF_PER0(odven) ADD_PER0(ship_archi.odvenbase));
    fprintf(fdes, PRF_PER20(odven) ADD_PER20(ship_archi.odvenbase));
    fprintf(fdes, PRF_PER40(odven) ADD_PER40(ship_archi.odvenbase));

    fprintf(fdes, "ship density %f explosion damage %f \n", ship_archi.dens,
            ship_archi.explr);
    fprintf(fdes, "\n");
    fprintf(fdes, "offensive combat mass %f \n", ship_archi.ocvmass);
    fprintf(fdes, "offensive combat base %f \n", ship_archi.ocvbase);
    fprintf(fdes, PRF_PER0(ocv) ADD_PER0(ship_archi.ocvbase));
    fprintf(fdes, PRF_PER20(ocv) ADD_PER20(ship_archi.ocvbase));
    fprintf(fdes, PRF_PER40(ocv) ADD_PER40(ship_archi.ocvbase));
    fprintf(fdes, "\n");

    fprintf(fdes, "energy weapon range parameters \n");
    fprintf(fdes, "short @0per %f mid @0per %f long @0per %f \n",
            ship_archi.ocvbase * 100.0, ship_archi.ocvbase * 10000.0,
            ship_archi.ocvbase * 100000.0);
    fprintf(fdes, "short @05per %f mid @05per %f long @05per %f \n",
            (ship_archi.ocvbase + fes[PER05])* 100.0, (ship_archi.ocvbase +
                    fes[PER05])* 10000.0, (ship_archi.ocvbase + fes[PER05]) * 100000.0);
    fprintf(fdes, "short @10per %f mid @10per %f long @10per %f \n",
            (ship_archi.ocvbase + fes[PER10])* 100.0, (ship_archi.ocvbase +
                    fes[PER10])* 10000.0, (ship_archi.ocvbase + fes[PER10]) * 100000.0);
    fprintf(fdes, "short @12per %f mid @12per %f long @12per %f \n",
            (ship_archi.ocvbase + fes[PER12])* 100.0, (ship_archi.ocvbase +
                    fes[PER12])* 10000.0, (ship_archi.ocvbase + fes[PER12]) * 100000.0);
    fprintf(fdes, "short @15per %f mid @15per %f long @15per %f \n",
            (ship_archi.ocvbase + fes[PER15])* 100.0, (ship_archi.ocvbase +
                    fes[PER15])* 10000.0, (ship_archi.ocvbase + fes[PER15]) * 100000.0);
    fprintf(fdes, "short @20per %f mid @20per %f long @20per %f \n",
            (ship_archi.ocvbase + fes[PER20])* 100.0, (ship_archi.ocvbase +
                    fes[PER20])* 10000.0, (ship_archi.ocvbase + fes[PER20]) * 100000.0);
    fprintf(fdes, "short @23per %f mid @23per %f long @23per %f \n",
            (ship_archi.ocvbase + fes[PER23])* 100.0, (ship_archi.ocvbase +
                    fes[PER23])* 10000.0, (ship_archi.ocvbase + fes[PER23]) * 100000.0);
    fprintf(fdes, "short @30per %f mid @30per %f long @30per %f \n",
            (ship_archi.ocvbase + fes[PER30])* 100.0, (ship_archi.ocvbase +
                    fes[PER30])* 10000.0, (ship_archi.ocvbase + fes[PER30]) * 100000.0);
    fprintf(fdes, "short @35per %f mid @35per %f long @35per %f \n",
            (ship_archi.ocvbase + fes[PER35])* 100.0, (ship_archi.ocvbase +
                    fes[PER35])* 10000.0, (ship_archi.ocvbase + fes[PER35]) * 100000.0);
    fprintf(fdes, "short @40per %f mid @40per %f long @40per %f \n",
            (ship_archi.ocvbase + fes[PER40])* 100.0, (ship_archi.ocvbase +
                    fes[PER40])* 10000.0, (ship_archi.ocvbase + fes[PER40]) * 100000.0);
    fprintf(fdes, "short @45per %f mid @45per %f long @45per %f \n",
            (ship_archi.ocvbase + fes[PER45])* 100.0, (ship_archi.ocvbase +
                    fes[PER45])* 10000.0, (ship_archi.ocvbase + fes[PER45]) * 100000.0);
    fprintf(fdes, "short @50per %f mid @50per %f long @50per %f \n",
            (ship_archi.ocvbase + fes[PER50])* 100.0, (ship_archi.ocvbase +
                    fes[PER50])* 10000.0, (ship_archi.ocvbase + fes[PER50]) * 100000.0);
    fprintf(fdes, "short @90per %f mid @90per %f long @90per %f \n",
            (ship_archi.ocvbase + fes[PER90])* 100.0, (ship_archi.ocvbase +
                    fes[PER90])* 10000.0, (ship_archi.ocvbase + fes[PER90]) * 100000.0);

    fprintf(fdes, "\n");
    fprintf(fdes, "defensive combat mass %f \n", ship_archi.dcvmass);
    fprintf(fdes, "defensive combat base %f \n", ship_archi.dcvbase);
    fprintf(fdes, PRF_PER0(dcv) ADD_PER0(ship_archi.dcvbase));
    fprintf(fdes, PRF_PER20(dcv) ADD_PER20(ship_archi.dcvbase));
    fprintf(fdes, PRF_PER40(dcv) ADD_PER40(ship_archi.dcvbase));


    fprintf(fdes, "\n");
    fprintf(fdes, "defensive screen mass %f \n", ship_archi.dscmass);
    fprintf(fdes, "defensive screen base %f \n", ship_archi.dscbase);
    fprintf(fdes, PRF_PER0(dsc) SCAL_PER0(ship_archi.dscbase));
    fprintf(fdes, PRF_PER20(dsc) SCAL_PER20(ship_archi.dscbase));
    fprintf(fdes, PRF_PER40(dsc) SCAL_PER40(ship_archi.dscbase));

    fprintf(fdes, "\n");
    fprintf(fdes, "internal energy units %f \n", ship_archi.isyseunits);
    fprintf(fdes, "internal energy mass  %f \n", ship_archi.isysmass);
    fprintf(fdes, "\n");
    fprintf(fdes, "cryogenic energy units %f \n", ship_archi.cryeunits);
    fprintf(fdes, "cryogenic energy mass %f \n",  ship_archi.crymass);
    fprintf(fdes, "\n");
    fprintf(fdes, "floor mass %f \n", ship_archi.flmass);
    fprintf(fdes, "hull damage value %f \n", ship_archi.hudv);
    fprintf(fdes, "hull pips %f \n", ship_archi.hpips);
    fprintf(fdes, "hull mass %f \n", ship_archi.humass);

    fprintf(fdes, "\n");
    fprintf(fdes, "contra-gravity mass %f \n", ship_archi.cgmass);

    if(ship_archi.tl < 15.0) {
        fprintf(fdes, "\nfussion micro atomic technology for gravatics \n");
        fprintf(fdes, "contra-gravity eunits %f \n", ship_archi.cgunits);
        fprintf(fdes, "contra-gravity range scalar %f \n", ship_archi.cgrange);
    } else {
        if(ship_archi.tl < 19.0) {
            fprintf(fdes, "\nmicro atomic technology plus partial direct matter to");
            fprintf(fdes, "energy gravatics \n");
        } else
            fprintf(fdes, "\ndirect matter to energy garvatics \n");

        fprintf(fdes, PRF_PER0(cg) SCAL_PER0(ship_archi.cgunits));
        fprintf(fdes, PRF_PER20(cg) SCAL_PER20(ship_archi.cgunits));
        fprintf(fdes, PRF_PER40(cg) SCAL_PER40(ship_archi.cgunits));

        fprintf(fdes, "\n");
        fprintf(fdes, PRF_PER0(cg-range) SCAL_PER0(ship_archi.cgrange));
        fprintf(fdes, PRF_PER20(cg-range) SCAL_PER20(ship_archi.cgrange));
        fprintf(fdes, PRF_PER40(cg-range) SCAL_PER40(ship_archi.cgrange));

    }

    fprintf(fdes, "\n");
    fprintf(fdes, "total ship mass %f \n", ship_archi.tsm);
    fprintf(fdes, "base ship mass %f \n", ship_archi.bsm);
    fprintf(fdes, "\n");
    fprintf(fdes, "hyperdrive mass %f \n", ship_archi.jumass);
    fprintf(fdes, "hyperdrive jump base %f \n", ship_archi.jubase);

    if(ship_archi.tl < 15.0) {
        fprintf(fdes, "\n");
        fprintf(fdes, "fussion micro atomic technology for star drive \n");
        fprintf(fdes, PRF_PER0(ju) SCAL_PER0(ship_archi.jubase));
        fprintf(fdes, PRF_PER20(ju) SCAL_PER20(ship_archi.jubase));
        fprintf(fdes, PRF_PER40(ju) SCAL_PER40(ship_archi.jubase));
    } else {
        if(ship_archi.tl < 19.0) {
            fprintf(fdes, "\n");
            fprintf(fdes, "micro atomic plus partial direct matter to energy ");
            fprintf(fdes, "stardrive \n");
            fprintf(fdes, PRF_PER0(ju-1day-fixed) SCAL_PER0(ship_archi.jubase));
            fprintf(fdes, PRF_PER20(ju-1day-fixed) SCAL_PER20(ship_archi.jubase));
            fprintf(fdes, PRF_PER40(ju-1day-fixed) SCAL_PER40(ship_archi.jubase));
            fprintf(fdes, PRF_PER0(ju-5day-fixed) SCAL_PER0(ship_archi.jubase * 5));
            fprintf(fdes, PRF_PER20(ju-5day-fixed) SCAL_PER20(ship_archi.jubase * 5));
            fprintf(fdes, PRF_PER40(ju-5day-fixed) SCAL_PER40(ship_archi.jubase * 5));
            fprintf(fdes, PRF_PER0(ju-20day-fixed) SCAL_PER0(ship_archi.jubase * 20));
            fprintf(fdes, PRF_PER20(ju-20day-fixed) SCAL_PER20(ship_archi.jubase * 20));
            fprintf(fdes, PRF_PER40(ju-20day-fixed) SCAL_PER40(ship_archi.jubase * 20));
        } else {
            fprintf(fdes, "\n");
            fprintf(fdes, "direct energy to matter stardrive \n");
            fprintf(fdes, PRF_PER0(ju-par-hour) SCAL_PER0(ship_archi.jubase));
            fprintf(fdes, PRF_PER20(ju-par-hour) SCAL_PER20(ship_archi.jubase));
            fprintf(fdes, PRF_PER40(ju-par-hour) SCAL_PER40(ship_archi.jubase));
        }
    }

    fprintf(fdes, "\n jump parameters at two times normal mass \n");

    if(ship_archi.tl < 15.0) {
        fprintf(fdes, "\n");
        fprintf(fdes, "fussion micro atomic technology for star drive \n");
        fprintf(fdes, PRF_PER0(ju) SCAL_PER0(ship_archi.jubase2));
        fprintf(fdes, PRF_PER20(ju) SCAL_PER20(ship_archi.jubase2));
        fprintf(fdes, PRF_PER40(ju) SCAL_PER40(ship_archi.jubase2));
    } else {
        if(ship_archi.tl < 19.0) {
            fprintf(fdes, "\n");
            fprintf(fdes, "micro atomic plus partial direct matter to energy ");
            fprintf(fdes, "stardrive \n");
            fprintf(fdes, PRF_PER0(ju-1day-fixed) SCAL_PER0(ship_archi.jubase2));
            fprintf(fdes, PRF_PER20(ju-1day-fixed) SCAL_PER20(ship_archi.jubase2));
            fprintf(fdes, PRF_PER40(ju-1day-fixed) SCAL_PER40(ship_archi.jubase2));
            fprintf(fdes, PRF_PER0(ju-5day-fixed) SCAL_PER0(ship_archi.jubase2 * 5));
            fprintf(fdes, PRF_PER20(ju-5day-fixed) SCAL_PER20(ship_archi.jubase2 * 5));
            fprintf(fdes, PRF_PER40(ju-5day-fixed) SCAL_PER40(ship_archi.jubase2 * 5));
            fprintf(fdes, PRF_PER0(ju-20day-fixed) SCAL_PER0(ship_archi.jubase2 * 20));
            fprintf(fdes, PRF_PER20(ju-20day-fixed) SCAL_PER20(ship_archi.jubase2 * 20));
            fprintf(fdes, PRF_PER40(ju-20day-fixed) SCAL_PER40(ship_archi.jubase2 * 20));
        } else {
            fprintf(fdes, "\n");
            fprintf(fdes, "direct energy to matter stardrive \n");
            fprintf(fdes, PRF_PER0(ju-par-hour) SCAL_PER0(ship_archi.jubase2));
            fprintf(fdes, PRF_PER20(ju-par-hour) SCAL_PER20(ship_archi.jubase2));
            fprintf(fdes, PRF_PER40(ju-par-hour) SCAL_PER40(ship_archi.jubase2));
        }
    }

    fprintf(fdes, "\n jump parameters at 5 times normal mass \n");

    if(ship_archi.tl < 15.0) {
        fprintf(fdes, "\n");
        fprintf(fdes, "fussion micro atomic technology for star drive \n");
        fprintf(fdes, PRF_PER0(ju) SCAL_PER0(ship_archi.jubase5));
        fprintf(fdes, PRF_PER20(ju) SCAL_PER20(ship_archi.jubase5));
        fprintf(fdes, PRF_PER40(ju) SCAL_PER40(ship_archi.jubase5));
    } else {
        if(ship_archi.tl < 19.0) {
            fprintf(fdes, "\n");
            fprintf(fdes, "micro atomic plus partial direct matter to energy ");
            fprintf(fdes, "stardrive \n");
            fprintf(fdes, PRF_PER0(ju-1day-fixed) SCAL_PER0(ship_archi.jubase5));
            fprintf(fdes, PRF_PER20(ju-1day-fixed) SCAL_PER20(ship_archi.jubase5));
            fprintf(fdes, PRF_PER40(ju-1day-fixed) SCAL_PER40(ship_archi.jubase5));
            fprintf(fdes, PRF_PER0(ju-5day-fixed) SCAL_PER0(ship_archi.jubase5 * 5));
            fprintf(fdes, PRF_PER20(ju-5day-fixed) SCAL_PER20(ship_archi.jubase5 * 5));
            fprintf(fdes, PRF_PER40(ju-5day-fixed) SCAL_PER40(ship_archi.jubase5 * 5));
            fprintf(fdes, PRF_PER0(ju-20day-fixed) SCAL_PER0(ship_archi.jubase5 * 20));
            fprintf(fdes, PRF_PER20(ju-20day-fixed) SCAL_PER20(ship_archi.jubase5 * 20));
            fprintf(fdes, PRF_PER40(ju-20day-fixed) SCAL_PER40(ship_archi.jubase5 * 20));
        } else {
            fprintf(fdes, "\n");
            fprintf(fdes, "direct energy to matter stardrive \n");
            fprintf(fdes, PRF_PER0(ju-par-hour) SCAL_PER0(ship_archi.jubase5));
            fprintf(fdes, PRF_PER20(ju-par-hour) SCAL_PER20(ship_archi.jubase5));
            fprintf(fdes, PRF_PER40(ju-par-hour) SCAL_PER40(ship_archi.jubase5));
        }
    }

    if(ship_archi.tl < 19.0) {
        fprintf(fdes, "\n");
        fprintf(fdes, "manuever drive base %f \n", ship_archi.madgbase);
        fprintf(fdes, PRF_PER0(madg) SCAL_PER0(ship_archi.madgbase / ship_archi.tsm));
        fprintf(fdes, PRF_PER20(madg) SCAL_PER20(ship_archi.madgbase / ship_archi.tsm));
        fprintf(fdes, PRF_PER40(madg) SCAL_PER40(ship_archi.madgbase / ship_archi.tsm));
        fprintf(fdes, "manuever drive mass %f \n", ship_archi.madmass);
        fprintf(fdes, "\n manuever drive at 2 times ship tonnage \n");
        fprintf(fdes, PRF_PER0(madg) SCAL_PER0(ship_archi.madgbase / (ship_archi.tsm * 2)));
        fprintf(fdes, PRF_PER20(madg) SCAL_PER20(ship_archi.madgbase / (ship_archi.tsm * 2)));
        fprintf(fdes, PRF_PER40(madg) SCAL_PER40(ship_archi.madgbase / (ship_archi.tsm * 2)));
        fprintf(fdes, "manuever drive base at 5 times ship tonnage \n");
        fprintf(fdes, PRF_PER0(madg) SCAL_PER0(ship_archi.madgbase / (ship_archi.tsm * 5)));
        fprintf(fdes, PRF_PER20(madg) SCAL_PER20(ship_archi.madgbase / (ship_archi.tsm * 5)));
        fprintf(fdes, PRF_PER40(madg) SCAL_PER40(ship_archi.madgbase / (ship_archi.tsm * 5)));
    }

    fprintf(fdes, "\n short range scan range \n");
    fprintf(fdes, PRF_PER0(sc) ADD_PER0(ship_archi.scbase));
    fprintf(fdes, PRF_PER20(sc) ADD_PER20(ship_archi.scbase));
    fprintf(fdes, PRF_PER40(sc) ADD_PER40(ship_archi.scbase));

    CONF(Green One subspace, PER05, PER05, PER05, PER15, PER25, PER10, PER0, PER10, PER25);
    CONF(Green Two subspace, PER05, PER05, PER05, PER15, PER50, PER25, PER0, PER05, PER0);
    CONF(Green One hyperspace, PER05, PER05, PER05, PER05, PER05, PER05, PER30, PER05, PER35);
    CONF(Green Two hyperspace, PER0, PER0, PER0, PER0, PER0, PER05, PER90, PER05, PER0);
    CONF(Yellow One, PER12, PER12, PER12, PER12, PER12, PER12, PER0, PER05, PER23);
    CONF(Yellow Two, PER25, PER12, PER12, PER25, PER05, PER05, PER0, PER05, PER12);
    CONF(Yellow Three, PER25, PER12, PER23, PER25, PER05, PER05, PER0, PER05, PER0);
    CONF(Red One, PER10, PER05, PER30, PER30, PER10, PER0, PER0, PER0, PER15);
    CONF(Red Two, PER15, PER05, PER35, PER35, PER10, PER0, PER0, PER0, PER0);
    CONF(Red Three, PER0, PER0, PER50, PER45, PER05, PER0, PER0, PER0, PER0);
    CONF(Black One, PER35, PER40, PER05, PER05, PER05, PER05, PER0, PER05, PER0);
    CONF(Gray One, PER10, PER10, PER10, PER10, PER10, PER10, PER10, PER10, PER20);
    CONF(Blue One, PER05, PER05, PER05, PER05, PER05, PER30, PER30, PER0, PER15);
    fprintf(fdes, "\n energy units at 1 percen %d \n", (((int)(fes[PER05] / 5.0) + (GRAN /2))
            / GRAN ) * GRAN);
    fflush(fdes);
}



