CFLAGS =        -g -Wall -pedantic -Wstrict-prototypes
#CFLAGS =       -O2 -Wall -ansi -pedantic -Wstrict-prototypes    

sound: main.c fft.c
	gcc main.c fft.c -o sound -lm

clean:
	rm -f sound
