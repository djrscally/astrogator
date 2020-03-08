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

### How's this actually work?

For getting known positions of natural bodies; lookup the appropriate values in the ephemeris. 

For everything else, it's a bit more complicated. 

#### Fixing the spacecraft's position

This is some fancy geometry, basically. We measure the angles between an _origin_ and two other solar system bodies. For argument's sake, let's call them the Sun, plus Venus and the Earth. Let's first consider the Spacecraft -> Sun -> Venus fix. The three points give us a triangle, of which we know one angle (because we measured it) and one side (the Sun -> Venus side, because the ephemeris tells us what that is).

_pic goes here_

We can't solve that triangle with what we have; insufficient data. Instead, we're going to construct the circle that touches all three of the points in question (meaning, we're going to calculate the centre of that circle and its radius). The first step to do so is to build a new triangle, with the point corresponding to the spacecraft moved to the (currently unknown) centre of the circle. This is a valid transformation because an [inscribed angle](https://en.wikipedia.org/wiki/Inscribed_angle) is twice the central angle that subtends the same arc on the circle - because we measured the Sun -> Venus angle we know that the angle of that triangle with the point moved to the circle's centre is double whatever our measurement was:

_next pic_

At this point, we split the Sun -> Venus line in two perpendicularly; remember that we know how long that line is and so we know where to split it. This splits our new triangle into two right angle triangles:

_final pic_

And from here, it's reasonably easy trigonometry. The tan of an angle is the length of the opposite side over the length of the adjacent side, i.e. - we know the angle A and the length of the opposite side, and so we can rearrange the equation to calculate the length of the adjecent side. This tells us exactly where the centre of the circle is. Next, the sin of an angle is the length of the opposite side over the hypotenuse (which is, in this case, the radius of the circle). Rearranging that gives us XYZ to get the hypotenuse, and so we know the circle's radius too.

The operation is identical on whichever other body was selected, and the penultimate stage is to have two known circles, with two intersections. The first intersection will be at our _origin_ - in this case the sun. The second will be at the _spacecraft_, which is what we want to know!

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