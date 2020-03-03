# Astrogator
## A program for obtaining positional fixes and determining orbital parameters within the Solar System, using a sextant.

#### Err, what?

I know, I know. Probably useless right? But it's a fun project that I've been wanting to do for a while, gives me an excuse to learn to use C and also builds groundwork for a much more useful project I'm planning in the future.

The basic principles are from Arthur C Clarke's _The Exploration of Space_, of which there is a summary of the pertinent section on [Project RHO](http://www.projectrho.com/public_html/rocket/astrodeck.php#keepingontrack)

#### How could you even check to see if this worked?

Well...

1. You're in space right now, you just happen to be on the earth. Use the software to ascertain the Earth's position in its orbit and compare it to the position it reports for Earth (which is gleaned from JPL's ephemeris)
2. [Orbiter](http://orbit.medphys.ucl.ac.uk/download.html) and [KSP](https://www.kerbalspaceprogram.com/) should both let you trial it reasonably accurately.

#### Installation

Binary on github, or compile from source. Compilation should be as simple as `make`, `make install`

#### To-Do

1. Fix from angles in 2D
2. Expand fixes to 3D
3. Calculate Orbital Parameters from fixes
4. Additionally allow distance estimation from angular separation of each limb of a planet.