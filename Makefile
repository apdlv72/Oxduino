
# Assume ino file has the same name as the directory
NAME=$(shell basename "$$PWD")


all:
	gcc -DOSX -x c++ -I osx -o $(NAME) $(NAME).ino


