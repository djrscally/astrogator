#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <argp.h>
#include <argz.h>

#include "novas/solarsystem.h"
#include "novas/novas.h"
#include "novas/eph_manager.h"
#include "astrogator.h"

struct argp_option options[] = {
    {"mode", 'm', "MODE", 0, "Switch program into position/orbit/range mode."},
    {"get", 'g', "BODY", 0, "Get data for the specified natural body rather than the user's spacecraft."},
    {"datetime", 'd', "DATETIME", 0, "Use specified datetime (in format YYYY-m-d-HH)"},
    {"position", 999, 0, 0, "Shorthand for --mode=position"},
    {"orbit", 888, 0, 0, "Shorthand for --mode=orbit"},
    {"range", 777, 0, 0, "Shorthand for --mode=range"},
    { 0 }
};

struct argp argp = {
    options,
    parse_args,
    "-m p -g BODY\n-m p ORIGIN BODY1 ANGLE1 BODY2 ANGLE2\n-m o -g BODY\n-m o POSITION POSITION\n-m r BODY ANGLE",
    "\nObtain astronomical navigation data.\vSee https://github.com/djrscally/astrogator for full user guide. Positional data drawn from JPL's ephemerides."
};

int
main(int argc, char * argv[])
{
    struct arg_flags af;

    // parse the arguments into our various flags
    argp_parse(&argp, argc, argv, 0, 0, &af);

    if (af.mode == 'p') {
        if (af.get_flag) {
            // We're in get known position of object mode. First up, parse datetime into
            // a julian date

            double tjd;
            // did we pass a specific datetime?
            if (af.custom_dt) {
                tjd = julian_date(af.dt_year, af.dt_month, af.dt_day, af.dt_hour);
                printf("%.6lf", tjd);
            } else { // you presumably want the current position
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);

                tjd = julian_date(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour);
            }

            // output variables
            double position[3];
            double velocity[3];

            int result = get_position(af.type, af.body1, tjd, position, velocity);

            if (!result) {
                printf("position: (%.6lf, %.6lf, %.6lf), velocity: (%.6lf, %.6lf, %.6lf)\n",
                    position[0], position[1], position[2],
                    velocity[0], velocity[1], velocity[2]
                );
            } else {
                printf("get_position returned %d\n", result);
                return result;
            }

        }
        return SUCCESS;
    }
}

int
get_position(int type, int number, double tjd, double position[3], double velocity[3])
/*
    get_position returns the position  and velocity of a body at the date indicated.

        Input Params
            type            = type of object
                                = 0 ... major planet, Pluto, Sun, or Moon
                                = 1 ... minor planet
            number          = object number
                                For 'type' = 0: Mercury = 1, ..., Pluto = 9,
                                                Sun = 10, Moon = 11
                                For 'type' = 1: minor planet number
            tjd             = julian date   
*/
{
    // we need a dummy cat entry to make the object
    cat_entry * cat = (cat_entry *) malloc(sizeof(cat_entry));
    make_cat_entry("DUMMY", "xxx",0,0.0,0.0,0.0,0.0,0.0,0.0,cat);

    // make the body that was requested into an object
    object * body = (object *) malloc(sizeof(object));
    make_object(type, number, "Temp", cat, body);

    // construct the julian date array
    double tjd_arr[2] = {tjd, 0.};

    // outs for ephem_open
    double jdb;
    double jde;
    short int den;

    // open the ephemeris
    int ephem_status = ephem_open("novas/JPLEPH", &jdb, &jde, &den);

    if (ephem_status) {
        printf("ERROR: The Ephemeris could not be opened correctly.\n");
        switch (ephem_status) {
            case 1:
                printf("The specified file does not exist / cannot be found\n");
                break;
            case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:
                printf("There was an error reading from the file header.\n");
                break;
            case 11:
                printf("Unable to set record length. Provided DE Number not in lookup table\n");
                break;
            default:
                printf("An undefined exception occurred. ephem_open returned the status code %d\n", ephem_status);
        }
        // 100 defines an error opening the ephemeris
        return ERROR_GETPOSITION + 100 + ephem_status;
    }

    int ephem_result = ephemeris(tjd_arr, body, 0,0, position, velocity);
    ephem_close();

    if (ephem_result) {
        printf("ERROR: The Ephemeris returned the following error code: %d\n", ephem_result);
        // 200 defines an error returned by the ephemeris
        return ERROR_GETPOSITION + 200 + ephem_result;
    }

    // cleanup memory
    free(body);
    free(cat);

    return 0;
}

void
fix_position(void)
{
    double position[3];
    double velocity[3];
    double tjd;

    cat_entry * cat = (cat_entry *) malloc(sizeof(cat_entry));
    make_cat_entry ("DUMMY","xxx",0,0.0,0.0,0.0,0.0,0.0,0.0,cat);
    
    tjd = julian_date(2020, 3, 2, 23);
    printf("\nTJF: %.6lf", tjd);

    double tjd_arr[2] = {tjd, 0};

    object mars = {0, 4, "Mars", *cat};
    int result;

    // outs for ephem_open
    double jdb;
    double jde;
    short int den;

    /* open the ephemeris */
    int eph_status = ephem_open("novas/JPLEPH.421", &jdb, &jde, &den);
    printf("\n Eph Status: %d", eph_status);
    printf("\nJDB: %.2lf, JDE: %.2lf, DE Num: %d", jdb, jde, den);

    result = ephemeris(tjd_arr, &mars, 0,0, position, velocity);

    ephem_close();

    printf("\nEphemeris Return Code: %d", result);

    printf("\nMars Position: (%.6lf, %.6lf, %.6lf)", position[0], position[1], position[2]);

    free(cat);
}