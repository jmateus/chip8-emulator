Chip-8 Emulator
==============

A simple Chip-8 emulator, written in C. 

It was only tested on Windows, but will most likely work on Linux if the MinGW lib is removed from the makefile.


Requirements
------------

+ MinGW
+ SDL 2


Compile, test and run
---------------

To compile, just run `make`. 
To run all the unit tests, run `make test`.
To start the emulator, run the command `chip 8 <program>`, with `<program>` being the path to a ch8 file.


Configuration file
------------------

The configuration file, `config.ini`, has the following parameters:

+ Scale: Resolution of the emulator window in relation to the original Chip-8 resolution (64x32).
+ CPU Clock Rate: Speed of the emulation, in MHz.
+ Keys: Configuration of the Chip-8 control scheme


TODO
----

+ Implement the sound module
+ Add more options to the config.ini
+ Write the missing tests, especially for the graphics and input modules
+ Add a toolbar to the emulator window
+ Test on Linux