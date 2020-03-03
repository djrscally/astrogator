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

    if (strcmp(argv[1], "position") == 0) {
        program_state = 0; // placeholder, should be fix_position(body a, body b, angle, datetime);
    } else if (strcmp(argv[1], "orbit") == 0) {
        // We're fixing orbits
    } else if (strcmp(argv[1], "range") == 0) {
        // We're estimating range
    } else if (strcmp(argv[1], "--help") == 0) {
        show_help();
        return 0;
    } else if (strcmp(argv[1], "--version") == 0) {
        program_state = show_version();
    }
    else {
        printf("\nERROR: An invalid mode was selected. Try astrogator --help for usage information.");
        return ERROR_INVALIDMODE;
    }

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

}

int
fix_position(object body_a, object body_b, double tjd, double angle, double[3] position, double[3] velocity)
{

}