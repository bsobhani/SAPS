CFLAGS =        -g -Wall -ansi -pedantic -Wstrict-prototypes
#CFLAGS =       -O2 -Wall -ansi -pedantic -Wstrict-prototypes    

lpf: lpf.c fft.c file_io.c
	gcc $(CFLAGS) lpf.c fft.c file_io.c -o lpf -lm

clean:
	rm -f lpf
