#include "structs.h"

/*----------------------------------------------------------------------*/
/*  This function returns a random real number between the specified    */
/* inner and outer bounds.                                              */
/*----------------------------------------------------------------------*/

double 
random_number (double inner, double outer)
{
     double delta = fabs(outer - inner);
     if (inner < outer)
       return (inner + delta * genrand_real3());
     else
       return (outer + delta * genrand_real3());
}


/*----------------------------------------------------------------------*/
/*   This function returns a value within a certain variation of the    */
/*   exact value given it in 'value'.                                   */
/*----------------------------------------------------------------------*/

double 
about (double value, double variation)
{
  double inner = value - variation;
  return (inner + 2.0 * variation * genrand_real3());
}


double 
random_eccentricity (void)
{
     return(1.0 - pow((double)(genrand_real3()),ECCENTRICITY_COEFF));
}
