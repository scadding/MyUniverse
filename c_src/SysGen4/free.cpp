#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysgen.h"

void free2body(body *sys)
{
    body    *s;

    for(s = sys->b; s; s = s->o) {
        free2body(s);
    }
    free(sys);
}

void sys2free(sistem *syst)
{
    free2body(syst->sys);
}

void free3body(body *sys)
{
    body    *s;

    for(s = sys->b; s; s = s->o) {
        free2body(s);
    }
    if((sys->type == T_WORLD) || (sys->type == T_MAIN)) {
        while(sys->det.w.bc) {
            free(sys->det.w.bc);
            sys->det.w.bc = sys->det.w.bc->next;
        }
    }
    free(sys);
}

void sys3free(sistem *sys)
{
    free3body(sys->sys);
}
