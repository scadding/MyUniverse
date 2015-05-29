#include "structs.h"

void 
display_system (void)
{
     planet_pointer node1; 
     int counter; 

     FILE *f;
     f = fopen("New.System", "w");

     fprintf(f,"                         SYSTEM  CHARACTERISTICS\n");
     fprintf(f,"Mass of central star:          %6.3f solar masses\n", stellar_mass_ratio);
     fprintf(f,"Luminosity of central star:    %6.3f (relative to the sun)\n",stellar_luminosity_ratio);
     fprintf(f,"Total main sequence lifetime:  %6.0f million years\n", (main_seq_life / 1.0E6));
     fprintf(f,"Current age of stellar system: %6.0f million years\n",(age / 1.0E6));
     fprintf(f,"Radius of habitable ecosphere: %6.3f AU\n\n",r_ecosphere);

     node1 = planet_head;
     counter = 1;

     while (node1 != NULL) {
	  fprintf(f,"Planet #%d:\n",counter);
	  if (node1->gas_giant)
	       fprintf(f,"Gas giant...\n");
	  if (node1->resonant_period)
	       fprintf(f,"In resonant period with primary.\n");

	  fprintf(f,"   Distance from primary star (in A.U.): %7.3f\n",node1->a);
	  fprintf(f,"   Eccentricity of orbit:                %7.3f\n",node1->e);
	  fprintf(f,"   Mass (in Earth masses):               %7.3f\n",node1->mass * EARTH_MASSES_PER_SOLAR_MASS);
	  fprintf(f,"   Equatorial radius (in Km):            %7.1f\n",node1->radius);
	  fprintf(f,"   Density (in g/cc):                    %7.3f\n",node1->density);
	  fprintf(f,"   Escape Velocity (in km/sec):          %7.2f\n",node1->escape_velocity / CM_PER_KM);
	  fprintf(f,"   Smallest molecular weight retained:   %7.2f",node1->molecule_weight);
	  
	  if (node1->molecule_weight < MOLECULAR_HYDROGEN)
	      fprintf(f,"   (H2)\n");
	  else if (node1->molecule_weight < HELIUM)
	      fprintf(f,"   (He)\n");
	  else if (node1->molecule_weight < METHANE)
	      fprintf(f,"   (CH4)\n");
	  else if (node1->molecule_weight < AMMONIA)
	      fprintf(f,"   (NH3)\n");
	  else if (node1->molecule_weight < WATER_VAPOR)
	      fprintf(f,"   (H2O)\n");
	  else if (node1->molecule_weight < NEON)
	      fprintf(f,"   (Ne)\n");
	  else if (node1->molecule_weight < MOLECULAR_NITROGEN)
	      fprintf(f,"   (N2)\n");
	  else if (node1->molecule_weight < CARBON_MONOXIDE)
	      fprintf(f,"   (CO)\n");
	  else if (node1->molecule_weight < NITRIC_OXIDE)
	      fprintf(f,"   (NO)\n");
	  else if (node1->molecule_weight < MOLECULAR_OXYGEN)
	      fprintf(f,"   (O2)\n");
	  else if (node1->molecule_weight < HYDROGEN_SULPHIDE)
	      fprintf(f,"   (H2S)\n");
	  else if (node1->molecule_weight < ARGON)
	      fprintf(f,"   (Ar)\n");
	  else if (node1->molecule_weight < CARBON_DIOXIDE)
	      fprintf(f,"   (CO2)\n");
	  else if (node1->molecule_weight < NITROUS_OXIDE)
	      fprintf(f,"   (N2O)\n");
	  else if (node1->molecule_weight < NITROGEN_DIOXIDE)
	      fprintf(f,"   (NO2)\n");
	  else if (node1->molecule_weight < OZONE)
	      fprintf(f,"   (O3)\n");
	  else if (node1->molecule_weight < SULPHUR_DIOXIDE)
	      fprintf(f,"   (SO2)\n");
	  else if (node1->molecule_weight < SULPHUR_TRIOXIDE)
	      fprintf(f,"   (SO3)\n");
	  else if (node1->molecule_weight < KRYPTON)
	      fprintf(f,"   (Kr)\n");
	  else if (node1->molecule_weight < XENON)
	      fprintf(f,"   (Xe)\n");
	  else
	      fprintf(f,"\n");
	  
	  fprintf(f,     "   Surface acceleration (in cm/sec2):    %7.2f\n",node1->surface_accel);
	  if (!(node1->gas_giant)) {
	    fprintf(f,"   Surface Gravity (in Earth gees):      %7.2f\n",node1->surface_grav);
	    if (node1->boil_point > 0.1)
	      fprintf(f,"   Boiling point of water (celcius):     %7.1f\n",
		      (node1->boil_point - KELVIN_CELCIUS_DIFFERENCE));
	    if (node1->surface_pressure > 0.00001) {
	      fprintf(f,"   Surface Pressure (in atmospheres):    %7.3f",(node1->surface_pressure / 1000.0));
	      if (node1->greenhouse_effect)
		fprintf(f,"     RUNAWAY GREENHOUSE EFFECT\n");
	      else 
		fprintf(f,"\n");
	    }
	       fprintf(f,"   Surface temperature (Celcius):        %7.2f\n",(node1->surface_temp - KELVIN_CELCIUS_DIFFERENCE));
	       if (node1->hydrosphere > 0.01)
		   fprintf(f,"   Hydrosphere percentage: %6.2f\n",(node1->hydrosphere * 100));
	       if (node1->cloud_cover > 0.01)
		   fprintf(f,"   Cloud cover percentage: %6.2f\n",(node1->cloud_cover * 100));
	       if (node1->ice_cover > 0.01)
		   fprintf(f,"   Ice cover percentage:   %6.2f\n",(node1->ice_cover * 100));
	  }
	  fprintf(f,"   Axial tilt (in degrees):   %7d\n",node1->axial_tilt);
	  fprintf(f,"   Planetary albedo:          %7.3f\n",node1->albedo);
	  fprintf(f,"   Length of year (in years): %7.2f\n", (node1->orbital_period / 365.25));
	  fprintf(f,"   Length of day (in hours):  %7.2f\n\n",node1->day);
	  counter++;
	  node1 = node1->next_planet;
     }
        fclose(f);
}
