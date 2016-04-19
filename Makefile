CFLAGS =        -g -Wall -pedantic -Wstrict-prototypes
#CFLAGS =       -O2 -Wall -ansi -pedantic -Wstrict-prototypes    

sound: lpf.c fft.c
	gcc lpf.c fft.c -o lpf -lm

clean:
	rm -f lpf
