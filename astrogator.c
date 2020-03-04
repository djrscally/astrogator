#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

    // argument flags, with defaults
    struct arg_flags af = {
        'p',
        0,
        0,
        1,
        1,
        0,
        "2020030422",
        0,
        0,
        0
    };

    // parse the arguments into our various flags
    parse_args(argc, argv, &af);

    // break and exit if user asks for help or version info
    if (af.help) {
        show_help();
        return SUCCESS;
    } else if (af.version) {
        show_version();
        return SUCCESS;
    }

    if (af.mode == 'p') {
        if (af.get_flag) {
            // We're in get known position of object mode. First up, parse datetime into
            // a julian date

            double tjd;
            // did we pass a specific datetime?
            if (af.custom_dt) {
                printf("Not implemented yet");
                tjd = julian_date(2020, 3, 4, 23);
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
parse_args(int argc, char * argv[], struct arg_flags * af)
{
    int c;

    while ((c = getopt(argc, argv, "m:o:b:gt:vh")) != -1) {
        switch (c) {
            case 'm':
                af->mode = *optarg;
                break;
            case 'g':
                af->get_flag = 1;
                break;
            case 't':
                af->type = atoi(optarg);
                break;
            case 'b':
                af->body1 = atoi(optarg);
                break;
            case 'v':
                af->version = 1;
                break;
            case 'h':
                af->help = 1;
                break;
            case '?':
                printf("INFO: Ignoring unrecognised option %c", (char) optopt);
        }
    }

    return 0;
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