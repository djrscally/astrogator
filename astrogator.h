// includes
#ifndef __STDIO__
    #include <stdio.h>
#endif

#ifndef __NOVAS__
    #include "novas/novas.h"
#endif

// macros
#define VERSION             "0.1.0"
#define ERROR_NOARGS        1
#define ERROR_INVALIDMODE   2

// function prototypes for header
int show_help(void);
int show_version(void);

// function prototypes for source
int fix_position(object, object, double, double, double[3], double[3]);

// helper functions for source file
int
show_help(void)
{
    printf("astrogator v"VERSION" compiled "__DATE__"\n");
    printf("Usage: astrogator --mode position <body [-g | body angle body body angle [body body angle...]]> [-dt yyyy-mm-dd hh:mm:ss] [-v]\n");
    printf("or   : astrogator --mode orbit <position position> [-dt yyyy-mm-dd hh:mm:ss] [-v]\n");
    printf("or   : astrogator --mode range body separation\n");
    printf("or   : astrogator --help\n");
    printf("or   : astrogator --version\n");
    printf("Position Mode Options:\n\
            -g              Get the known current position of a solar system body rather than calculate the position of a\
                            spacecraft.\
            -dt             Specify a datetime to use in calculating positions. Absent this option, the program will default\
                            to using the current datetime.\
            Orbit Mode Options:\n\
            -v              Verbose\n\
            -sigs           Show signatures calculated based on first 32k for each file\n\
            -rdonly         Apply to readonly files also (as opposed to skipping them)\n\
            -ref <filepat>  Following file pattern are files that are for reference, NOT\n\
                            to be eliminated, only used to check duplicates against\n\
            -z              Do not skip zero length files (zero length files are ignored\n\
                            by default)\n\
            -u              Do not print a warning for files that cannot be read\n\
            -p              Hide progress indicator (useful when redirecting to a file)\n\
            -j              Follow NTFS junctions and reparse points (off by default)\n\
            -listlink       hardlink list mode.  Not valid with -del, -bat, -hardlink,\n\
                            or -rdonly, options\n\
            filepat         Pattern for files.  Examples:\n\
                             c:\\**        Match everything on drive C\n\
                             c:\\**\\*.jpg  Match only .jpg files on drive C\n\
                             **\\foo\\**    Match any path with component foo\n\
                                           from current directory down\n\
           \n\
           ");

    return 0;
}

int
show_version(void)
{
    printf("\n\
        astrogator "VERSION" compiled "__DATE__"\n\
        Copyright (C) 2020 Dan Scally\n\
        License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
        This is free software: you are free to change and redistribute it.\n\
        There is NO WARRANTY, to the extent permitted by law.\n\
        \n\
        Written by Dan Scally.\
    ");

    return 0;
}
