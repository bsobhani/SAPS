# Sobhani Audio Processing Suite
A set of tools for audio manipulation. 

## Commands
* filter
* speed
* info

## Commands under development
* tempo
* pitch
* firfilter

## Building

Simply type "make".

To build experimental tempo command (not built by default), type "make tempo".

## A note on reading and writing to files

The programs in this suite read from stdin and write to stdout. To read from and write to files, you can do something like this:

>./program < inputFile > outputFile

See the examples.

## Usage examples

### filter

Perform highpass, lowpass, bandpass, or bandstop filtering of a signal.

Examples:

* Lowpass filter - remove frequencies above 1000Hz:

  >./filter -l 1000 < horn.wav > hornFiltered.wav
  
* Highpass filter - remove frequencies below 3000Hz:

  >./filter -h 3000 < horn.wav > hornFiltered.wav

* Bandstop filter - remove frequencies between 1000Hz and 3000Hz:

  >./filter -s 1000 3000 < horn.wav > hornFiltered.wav

* Bandpass filter - remove all frequencies except those between 1000Hz and 3000Hz:

  >./filter -p 1000 3000 < horn.wav > hornFiltered.wav

### speed

Speed up or slow down a sound file.

Examples:

* Speed up a file to 1.5 times its original speed:

  >./speed 1.5 < horn.wav > hornFast.wav

### info

Display header data of a WAV file. Note: This program does not take data from stdin; you must specify the filename as a commandline argument.

Examples:

  >./info horn.wav

