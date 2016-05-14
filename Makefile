CFLAGS =        -g -Wall -ansi -pedantic -Wstrict-prototypes
#CFLAGS =       -O2 -Wall -ansi -pedantic -Wstrict-prototypes    

all: filter speed echo info

filter: filter.c fft.c file_io.c window_functions.c
	gcc $(CFLAGS) $^ -o $@ -lm

speed: speed.c file_io.c
	gcc $(CFLAGS) $^ -o $@ -lm

tempo: tempo.c file_io.c
	gcc $(CFLAGS) $^ -o $@ -lm

echo: echo.c conv.c file_io.c
	gcc $(CFLAGS) $^ -o $@ -lm

info: info.c file_io.c
	gcc $(CFLAGS) $^ -o $@ -lm


clean:
	rm -f filter speed echo tempo info
