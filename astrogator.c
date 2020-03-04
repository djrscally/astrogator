#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "novas/solarsystem.h"
#include "novas/novas.h"
#include "novas/eph_manager.h"
#include "astrogator.h"

int
main(int argc, char * argv[])
{

    // if no arguments are entered, print the help.
    if (argc == 1) {
        show_help();
        return ERROR_NOARGS;
    }
    
    int program_state;

    if (!strcmp(argv[1], "position")) {
        if (!strcmp(argv[2], "-g")) {
            // We're in get known position of object mode
            double tjd = julian_date(2020, 3, 2, 23);

            // convert the inputs to actual integers
            int type = *argv[3] - '0';
            int number = *argv[4] - '0';

            // output variables
            double position[3];
            double velocity[3];

            int result = get_position(type, number, tjd, position, velocity);

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
    } else if (!strcmp(argv[1], "orbit")) {
        // We're fixing orbits
    } else if (!strcmp(argv[1], "range")) {
        // We're estimating range
    } else if (!strcmp(argv[1], "--help")) {
        show_help();
        return 0;
    } else if (!strcmp(argv[1], "--version")) {
        program_state = show_version();
    }
    else {
        printf("\nERROR: An invalid mode was selected. Try astrogator --help for usage information.");
        return ERROR_INVALIDMODE;
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
    int ephem_status = ephem_open("novas/JPLEPH.421", &jdb, &jde, &den);

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