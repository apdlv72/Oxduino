
# Assume ino file has the same name as the directory
NAME=$(shell basename "$$PWD")
#INCL=-Iosx

ARDUINO_HOME=$(HOME)/Documents/Arduino
ARDUINO_LIBS=$(ARDUINO_HOME)/libraries
INCL=-Iosx -I$(ARDUINO_LIBS)/eEEPROM

COPT=-Wgnu-designator
CXXOPT=-Wgnu-designator

#test:
#	ls -l $(ARDUINO_LIBS)

all:
	gcc -DOSX        -c $(INCL) -o osx/Arduino.o osx/Arduino.cpp
	gcc -DOSX        -c $(INCL) -o osx/eEEPROM.o $(ARDUINO_LIBS)/eEEPROM/eEEPROM.cpp
	gcc -DOSX        -c $(INCL) -o osx/Serial.o  osx/Serial.cpp
	gcc -DOSX        -c $(INCL) -o osx/RTClib.o  osx/RTClib.cpp
	gcc -DOSX        -c $(INCL) -o osx/Wire.o    osx/Wire.cpp
	gcc -Wno-gnu-designator -DOSX -x c++ -c $(INCL) -o $(NAME).o     $(NAME).ino
	gcc              -o $(NAME) $(NAME).o osx/Arduino.o osx/Serial.o osx/eEEPROM.o osx/Wire.o osx/RTClib.o
	
clean:
	rm -v osx/*.o
	rm -v $(NAME)
	

	

