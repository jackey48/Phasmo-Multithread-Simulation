FILES = ghost.o hunter.o room.o house.o evidence.o utils.o logger.o main.o 

all:		${FILES}
		-gcc -Wall -Wextra -g -o Final ghost.o hunter.o room.o house.o evidence.o utils.o logger.o main.o

ghost:		ghost.c
			-gcc -Wall -c ghost.c

room:		room.c
			-gcc -Wall -c room.c
			
hunter:		hunter.c
			-gcc -Wall -c hunter.c

house:		house.c
			-gcc -Wall -c house.c
			
evidence:	evidence.c
			-gcc -Wall -c evidence.c
			
utils:		utils.c
			-gcc -Wall -c utils.c
			
logger:		logger.c
			-gcc -Wall -c logger.c
			
main:		main.c
			-gcc -Wall -c main.c

clean:		
			-rm -f ${FILES} Final
		
