CFLAGS =        -g -Wall -ansi -pedantic -Wstrict-prototypes
#CFLAGS =       -O2 -Wall -ansi -pedantic -Wstrict-prototypes    

all: lpf hpf speed

lpf: lpf.c fft.c file_io.c window_functions.c
	gcc $(CFLAGS) lpf.c fft.c file_io.c window_functions.c -o lpf -lm

hpf: hpf.c fft.c file_io.c
	gcc $(CFLAGS) hpf.c fft.c file_io.c -o hpf -lm

speed: speed.c file_io.c
	gcc $(CFLAGS) $^ -o $@ -lm


clean:
	rm -f lpf hpf speed
