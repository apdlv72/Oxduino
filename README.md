Oxduino
=======

A set of files to compile Arduino sketches for the OSX platform for debugging and testing purposes.

Copy the subdirectory osx/ and the Makefile into the directory of your Arduino sketch.
(This is the directory with the .ino file). Run "make" and your Arduino sketch should be 
compiled into a native application for the OSX platform.

Only a very basic set of commands/libraries is supprted. For example, Serial.print will go to the consoel.
Most files below the osx directory were actually "stolen" from the Arduino/AVR environment and
modified here and there to compile on OSX.

This is still quick and dirty and hopefully evolve over time. However, it already turned out to be
quite convenient to do some basic test on a "real" computer before uploading the sketch to the uC.


