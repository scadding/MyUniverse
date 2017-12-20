#include "sysgen.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

int makedetails(sistem *syst, long x, long y, long z);
void cover(int cov[20][28],
           int land[20][28],
           int plates[20][28],
           body *s);
body *print(sistem *syst,
            char *name,
            int plates[20][28],
            int land[20][28],
            int cov[20][28]);
body   *pdlevel(body *s,
                char *name,
                int plates[20][28],
                int land[20][28],
                int cov[20][28]);

extern sistem  syst;

void map(body *s, int plate[20][28], int land[20][28], int cov[20][28])
{
    int     t, h;
    dworld  *d;

    xyzmagic(syst.x, syst.y, syst.z, MAP_MAGIC);
    d = &s->det.w;
    for(t = 0; t < 20; t++)
        for(h = 0; h < 28; h++) {
            plate[t][h] = 0;
        }
    fprintf(stderr, "%s:", s->name);
    plates(plate, d->nplates);
    fprintf(stderr, ".");
    if(d->water < 50)
        seas(plate, land, d->arch, d->isl, d->min,
             d->maj, d->water);
    else
        lands(plate, land, d->arch, d->isl, d->min,
              d->maj, 100 - d->water);
    fprintf(stderr, ".");
    cover(cov, land, plate, s);
    fprintf(stderr, ".\n");
}

body *makemap(long x, long y, long z, char *name, int plates[20][28],
              int land[20][28], int cov[20][28])
{
    if(makedetails(&syst, x, y, z) == 0) {
        return(0);
    }
    return(print(&syst, name, plates, land, cov));
}

body *print(sistem *syst,
            char *name,
            int plates[20][28],
            int land[20][28],
            int cov[20][28])
{
    printf("%s system\n", syst->mw->name);
    printf("Location %ld,%ld,%ld\n", syst->x, syst->y, syst->z);
    return(pdlevel(syst->sys, name, plates, land, cov));
}

body   *pdlevel(body *s,
                char *name,
                int plates[20][28],
                int land[20][28],
                int cov[20][28])
{
    body    *ret;

    if(!s) {
        return(s);
    }
    if(!strcmp(s->name, name)) {
        if((s->type == T_WORLD) || (s->type == T_MAIN)) {
            map(s, plates, land, cov);
        } else {
            printf("Can only map worlds\n");
        }
        return(s);
    }
    if(!s->b) {
        return(0);
    }
    for(s = s->b; s; s = s->o) {
        ret = pdlevel(s, name, plates, land, cov);
        if(ret) {
            break;
        }
    }
    return(ret);
}
