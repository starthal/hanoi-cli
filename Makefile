all:
	gcc -Wall -Ofast main.c -o hanoi
debug:
	gcc -Wall -g3 main.c -o hanoi 
