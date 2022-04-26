all: compile run dotfile

compile:
	gcc -Wall main.cpp -o main -lm

run: 
	./main

dotfile:
	dot -Tjpg treeGr.txt > treeGr.png