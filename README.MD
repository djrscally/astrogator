# Astrogator
## A program for obtaining positional fixes and determining orbital parameters within the Solar System, using a sextant.

### Err, what?

I know, I know. Probably useless right? But it's a fun project that I've been wanting to do for a while, gives me an excuse to learn to use C and also builds groundwork for a much more useful project I'm planning in the future.

The basic principles are from Arthur C Clarke's _The Exploration of Space_, of which there is a summary of the pertinent section on [Project RHO](http://www.projectrho.com/public_html/rocket/astrodeck.php#keepingontrack):

> Say Roger want's to fix the position of the Polaris. From the ephemeris he knows where Terra is, and thus the Sol-Terra line. The ephemeris also tells him where Venus is, and thus the Sol-Venus line. Roger uses the periscopic sextant to measure angle A and angle B [meaning the angle between the Sun and Venus and the angle between the Sun and Terra]. With simple geometry the Polaris' current position is fixed. Of course this is an approximation based on assuming that everything is in the plane of the ecliptic. If the course gets more three dimensional a third angle will be required.

### How could you even check to see if this worked?

Well...

1. You're in space right now, you just happen to be on the earth. Use the software to ascertain the Earth's position in its orbit. You'll need three planets to be visible at once, which unless you have a _really_ good telescope basically means waiting for Saturn, Mars and Jupiter, or else going for a twilight fix when Venus is around. You can then compare it to the position it reports for Earth (which is gleaned from JPL's ephemeris)
2. [Orbiter](http://orbit.medphys.ucl.ac.uk/download.html) and [KSP](https://www.kerbalspaceprogram.com/) should both let you trial it reasonably accurately.

### Installation

This is reasonably portable code. The only possibly missing dependencies (likely on Windows, unlikely on Linux. No clue for a Mac) would be argp.h and argz.h, which are part of [The GNU C Library](https://www.gnu.org/software/libc/) and can be downloaded there. If you have GCC installed (as most Linux distributions will do) then you can simply do:

```
$ cd /path/to/somewhere
$ git clone https://github.com/djrscally/astrogator
$ cd astrogator
$ make astrogator
$ sudo make install
```

On windows you need to install a C compiler because you don't get one by default. Popular choices include the open source [mingw64](https://mingw-w64.org/doku.php), which is basically a port of gcc (and thus the makefile will probably work fine) or [Microsoft's Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) (Expand "Tools for Visual Studio 20nn and find "Build Tools for Visual Studio 2019). If you've gone for MSVC tools then try something like the following:

```
PS cd /path/to/somewhere
PS git clone https://github.com/djrscally/astrogator
```
_Open the Developer Tools for Visual Studio prompt from Start Menu_
```
$ cd /path/to/somewhere/astrogator
$ cl /Wall novas.c eph_manager.c novascon.c solsys1.c readeph0.c nutation.c astrogator.c /Fe: astrogator.exe
```

If windows is missing argp.h and argz.h, just download those and place them into the astrogator directory.

A simple `astrogator --version` should verify the install. 

### Usage
```
Usage: astrogator [OPTION...] -m p -g BODY
  or:  astrogator [OPTION...] -m p ORIGIN BODY1 ANGLE1 BODY2 ANGLE2
  or:  astrogator [OPTION...] -m o -g BODY
  or:  astrogator [OPTION...] -m o POSITION POSITION
  or:  astrogator [OPTION...] -m r BODY ANGLE

Obtain astronomical navigation data.

  -d, --datetime=DATETIME    Use specified datetime (in format YYYY-m-d-HH)
  -g, --get=BODY             Get data for the specified natural body rather
                             than the user's spacecraft.
  -m, --mode=MODE            Switch program into position/orbit/range mode.
      --orbit                Shorthand for --mode=orbit
      --position             Shorthand for --mode=position
      --range                Shorthand for --mode=range
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

See https://github.com/djrscally/astrogator for full user guide. Positional
data drawn from JPL's ephemerides.

Report bugs to https://github.com/djrscally/astrogator/issues.
```
### Acknowledgements

### Jet Propulsion Laboratory's Ephemeris
[This](https://ssd.jpl.nasa.gov/?ephemerides) is the mathematical model explaining the positions and velocities of solar system bodies. We need to know where the planets _are_ so we can use geometry to ascertain where **we** are, so this is vital.

#### US Naval Observatory's NOVAS-C Library
No link because they're currently renovating their website. I'll try to remember to update this once it's back. This is the C code living inside the novas directory of the project, and basically provides an interface to JPLs ephemerides. At least, that's what I'm using it for, but it actually does a ton of cool nerd stuff that anyone with interests in programming and astronomy should definitely check out. They also supply a Python and Fortran version.

### To-Do

1. Fix from angles in 2D
2. Expand fixes to 3D
3. Calculate Orbital Parameters from fixes
4. Additionally allow distance estimation from angular separation of each limb of a planet.