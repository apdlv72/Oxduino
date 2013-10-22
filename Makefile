
# Assume ino file has the same name as the directory
NAME=$(shell basename "$$PWD")
INCL=-Iosx

all:
	gcc -DOSX        -c $(INCL) -o osx/Arduino.o osx/Arduino.cpp
	gcc -DOSX        -c $(INCL) -o osx/eEEPROM.o osx/eEEPROM.cpp
	gcc -DOSX        -c $(INCL) -o osx/Serial.o  osx/Serial.cpp
	gcc -DOSX -x c++ -c $(INCL) -o $(NAME).o     $(NAME).ino
	gcc -o $(NAME) $(NAME).o osx/Arduino.o osx/Serial.o osx/eEEPROM.o
	
clean:
	rm *.o

	

