# Phony install target
.PHONY: install
install:
	install astrogator /usr/bin

# Link the custom object and novas library into the binary.
astrogator: astrogator.o libnovas.a
	gcc -Wall -Wextra astrogator.o -L. -lnovas -lm -o astrogator

# Object file for my code
astrogator.o: astrogator.c astrogator.h
	gcc -Wall -Wextra -c astrogator.c astrogator.h

# Link the novas objects into a library
libnovas.a: novas.o eph_manager.o novascon.o solsys1.o readeph0.o nutation.o
	ar rcs libnovas.a novas.o eph_manager.o novascon.o solsys1.o readeph0.o nutation.o
	
# Objects for the NOVAS source code
novas.o: novas/novas.c novas/novas.h
	gcc -Wall -Wextra -c novas/novas.c novas/novas.h
	
eph_manager.o: novas/eph_manager.c novas/eph_manager.h
	gcc -Wall -Wextra -c novas/eph_manager.c novas/eph_manager.h

novascon.o: novas/novascon.c novas/novascon.h
	gcc -Wall -Wextra -c novas/novascon.c novas/novascon.h

solsys1.o: novas/solsys1.c novas/solarsystem.h
	gcc -Wall -Wextra -c novas/solsys1.c novas/solarsystem.h
	
readeph0.o: novas/readeph0.c
	gcc -Wall -Wextra -c novas/readeph0.c

nutation.o: novas/nutation.c novas/nutation.h
	gcc -Wall -Wextra -c novas/nutation.c novas/nutation.h