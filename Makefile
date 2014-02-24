
# Assume ino file has the same name as the directory
NAME=$(shell basename "$$PWD")
#INCL=-Iosx

ARDUINO_HOME=$(HOME)/Documents/Arduino
ARDUINO_LIBS=$(ARDUINO_HOME)/libraries
INCL=-Iosx -I$(ARDUINO_LIBS)/eEEPROM -I$(ARDUINO_LIBS)/NecIR

COPT=-Wgnu-designator 
CXXOPT=-Wgnu-designator 

all:
	g++ -g                      -DOSX        -c $(INCL) -o osx/Oxduino.o osx/Oxduino.cpp
	g++ -g                      -DOSX        -c $(INCL) -o osx/Arduino.o osx/Arduino.cpp
	g++ -g  -Wno-return-type    -DOSX        -c $(INCL) -o osx/Print.o   osx/Print.cpp
	g++ -g                      -DOSX        -c $(INCL) -o osx/Serial.o  osx/Serial.cpp
	g++ -g  -Wno-return-type    -DOSX        -c $(INCL) -o osx/Stream.o  osx/Stream.cpp
	g++ -g  -Wno-return-type    -DOSX        -c $(INCL) -o osx/WString.o osx/WString.cpp
	g++ -g                      -DOSX        -c $(INCL) -o osx/eEEPROM.o $(ARDUINO_LIBS)/eEEPROM/eEEPROM.cpp
	g++ -g                      -DOSX        -c $(INCL) -o osx/NecIR.o   $(ARDUINO_LIBS)/NecIR/NecIR.cpp
	g++ -g                      -DOSX        -c $(INCL) -o osx/RTClib.o  osx/RTClib.cpp
	g++ -g  -Wno-return-type    -DOSX        -c $(INCL) -o osx/Wire.o    osx/Wire.cpp
	g++ -g  -Wno-return-type    -DOSX        -c $(INCL) -o osx/FastSPI_LED.o osx/FastSPI_LED.cpp
	g++ -g  -Wno-return-type -Wno-gnu-designator -DOSX -x c++ -c $(INCL) -o $(NAME).o     $(NAME).ino
	g++ -g  -o $(NAME) $(NAME).o osx/Oxduino.o osx/Arduino.o osx/Print.o osx/Stream.o osx/Serial.o osx/WString.o \
				osx/eEEPROM.o osx/NecIR.o osx/RTClib.o osx/Wire.o osx/FastSPI_LED.o

clean:
	$(RM) -f -v osx/*.o
	$(RM) -f $(NAME)
	

	

