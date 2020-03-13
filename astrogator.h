// includes
#ifndef __STDIO__
    #include <stdio.h>
#endif

#ifndef __NOVAS__
    #include "novas/novas.h"
#endif

#ifndef __ARGP__
    #include <argp.h>
#endif

#ifndef __STRING__
    #include <string.h>
#endif

// macros
#define VERSION             "0.1.1"
#define SUCCESS             0
#define ERROR_NOARGS        1
#define ERROR_INVALIDMODE   2
#define ERROR_GETPOSITION   1000

// structs
struct arg_flags {
    char mode;              // switch between position, orbit and range
    int get_flag : 1;       // are we doing a lookup rather than a calculation
    int type;         // the type of body we're looking at
    int body1;         // the identifier of the body we're looking at
    int body2;
    int custom_dt : 1;      // use a custom datetime?
    int dt_year;
    int dt_month;
    int dt_day;
    int dt_hour;
    int origin;
    char * argz;
    size_t argz_len;
    int interactive  : 1 // interactively get the details or parse from argz
};

// constants
const char * argp_program_bug_address = "https://github.com/djrscally/astrogator/issues";
const char * argp_program_version = "astrogator "VERSION" compiled "__DATE__"\n"
                                    "A program for aiding in astronomical navigation.\n"
                                    "Copyright (C) 2020 Dan Scally\n"
                                    "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n"
                                    "This is free software: you are free to change and redistribute it.\n"
                                    "There is NO WARRANTY, to the extent permitted by law.\n"
                                    "\n"
                                    "Written by Dan Scally.";

// function prototypes for header
int get_body_number(char *);

// function prototypes for source
int fix_position(
                // inputs
                double, int, int, double, int, double,
                // output
                double *);
int get_position(
                // inputs
                int, int, double,
                // outputs
                double *, double *);
int parse_args(int, char *, struct argp_state *);
int parse_dt(char *, struct arg_flags *);
int get_diameter(int, int *);
int get_range(int, double, double *);

// header functions
int
parse_args(int key, char * arg, struct argp_state * state)
{
    struct arg_flags * af = state->input;

    switch(key) {
        case 'm':
            af->mode=arg[0];
            if (!((af->mode=='p') || (af->mode=='r') || (af->mode=='o'))) {
                argp_failure(state, 1, 0, "Mode must be one of p[osition], r[ange] or o[rbit].\n");
            }
            break;
        case 'l':
            af->get_flag = 1;
            af->type=0;

            if (isdigit(arg[0])) {
                af->body1=atoi(arg);
            } else {
                int body = get_body_number(arg);
                if (body) {
                    af->body1=body;
                } else {
                    argp_failure(state, 1, 0, "An invalid body name was entered");
                }
            }
            
            break;
        case 'd':
            af->custom_dt = 1;

            if (parse_dt(arg, af)) {
                argp_failure(state, 1, 0, "Invalid datetime entered");
            }
        case 'n':
            af->interactive = 0;
            break;
        case 999:
            af->mode='p';
            break;
        case 888:
            af->mode='o';
            break;
        case 777:
            af->mode='r';
            break;
        case ARGP_KEY_ARG:
            printf("Arg encountered");
            af->type = 0;
            printf("%c", *arg);
            af->body1 = atoi(arg);
            break;
        case ARGP_KEY_END:
            // do some argument processing here depending on the mode we're in
            break;
    }

    return 0;
}

int
parse_dt(char * dt, struct arg_flags * af)
{
    char * c;
    int i=0;

    for (i=0;i<4;i++) {
        c = strtok(i==0 ? dt : NULL, "-");
        if (c==NULL) {
            break;
        }
        switch (i) {
            case 0:
                af->dt_year = atoi(c);
                break;
            case 1:
                af->dt_month = atoi(c);
                break;
            case 2:
                af->dt_day = atoi(c);
                break;
            case 3:
                af->dt_hour = atoi(c);
                break;
            default:
                return 1;
        }
    }

    if (i == 4) {
        return 0;
    } else {
        return 1; // not enough elements entered
    }
}

int
get_body_number(char * body_name)
{
    char * bodies[11] = {
        "mercury", "venus", "earth","mars", "jupiter",
        "saturn", "uranus", "neptune", "pluto", "sun", "moon"
        };

    int i;

    for (i=0;i<11;i++) {
        if (!strcmp(body_name, bodies[i])) {
            return i+1;
        }
    }

    // body name not matched.
    return 0;
}