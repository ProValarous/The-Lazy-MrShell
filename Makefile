shell: main.o help.o cd.o
	gcc -o runShell main.o -lreadline
	gcc -o help help.o 
	gcc -o cd cd.o -lreadline
main.o: main.c
	gcc -O -Wall -c main.c
help.o: help.c
	gcc -O -Wall -c help.c
cd.o: cd.c
	gcc -O -Wall -c cd.c
clean:
	rm -f main.o parse.o help.o cd.o runShell help cd