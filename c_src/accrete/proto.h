/* utils.c */
double random_number(double inner, double outer);
double about(double value, double variation);
double random_eccentricity(void);
/* accrete.c */
void set_initial_conditions(double inner_limit_of_dust, double outer_limit_of_dust);
double stellar_dust_limit(double stellar_mass_ratio);
double innermost_planet(double stellar_mass_ratio);
double outermost_planet(double stellar_mass_ratio);
double inner_effect_limit(double a, double e, double mass);
double outer_effect_limit(double a, double e, double mass);
int dust_available(double inside_range, double outside_range);
void update_dust_lanes(double min, double max, double mass, double crit_mass, double body_inner_bound, double body_outer_bound);
double collect_dust(double last_mass, double a, double e, double crit_mass, dust_pointer dust_band);
double critical_limit(double orbital_radius, double eccentricity, double stellar_luminosity_ratio);
void accrete_dust(double *seed_mass, double a, double e, double crit_mass, double body_inner_bound, double body_outer_bound);
void coalesce_planetesimals(double a, double e, double mass, double crit_mass, double stellar_luminosity_ratio, double body_inner_bound, double body_outer_bound);
planet_pointer distribute_planetary_masses(double stellar_mass_ratio, double stellar_luminosity_ratio, double inner_dust, double outer_dust);
/* display.c */
void display_system(void);
/* enviro.c */
double luminosity(double mass_ratio);
int orbital_zone(double orbital_radius);
double volume_radius(double mass, double density);
double kothari_radius(double mass, double orbital_radius, int giant, int zone);
double empirical_density(double mass, double orbital_radius, int gas_giant);
double volume_density(double mass, double equatorial_radius);
double period(double separation, double small_mass, double large_mass);
double day_length(double mass, double radius, double orbital_period, double eccentricity, int giant);
int inclination(double orbital_radius);
double escape_vel(double mass, double radius);
double rms_vel(double molecular_weight, double orbital_radius);
double molecule_limit(double orbital_radius, double mass, double equatorial_radius);
double acceleration(double mass, double radius);
double gravity(double acceleration);
int greenhouse(int zone, double orbital_radius, double greenhouse_radius);
double vol_inventory(double mass, double escape_vel, double rms_vel, double stellar_mass, int zone, int greenhouse_effect);
double pressure(double volatile_gas_inventory, double equatorial_radius, double gravity);
double boiling_point(double surface_pressure);
double hydrosphere_fraction(double volatile_gas_inventory, double planetary_radius);
double cloud_fraction(double surface_temp, double smallest_MW_retained, double equatorial_radius, double hydrosphere_fraction);
double ice_fraction(double hydrosphere_fraction, double surface_temp);
double eff_temp(double ecosphere_radius, double orbital_radius, double albedo);
double green_rise(double optical_depth, double effective_temp, double surface_pressure);
double planet_albedo(double water_fraction, double cloud_fraction, double ice_fraction, double surface_pressure);
double opacity(double molecular_weight, double surface_pressure);
void iterate_surface_temp(planet_pointer *planet);
/* main.c */
void init(void);
void generate_stellar_system(void);
int main(void);
/* mtrng.c */
void init_genrand(unsigned long s);
void init_by_array(unsigned long init_key[], unsigned long key_length);
unsigned long genrand_int32(void);
long genrand_int31(void);
double genrand_real1(void);
double genrand_real2(void);
double genrand_real3(void);
double genrand_res53(void);
