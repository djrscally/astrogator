#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "novas/solarsystem.h"
#include "novas/novas.h"
#include "novas/eph_manager.h"

void main()
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

}