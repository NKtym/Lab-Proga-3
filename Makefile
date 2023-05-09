exp1: exp1.o
	gcc -Wall -O0 -g  -o exp1 exp1.o
exp2: exp2.o
	gcc -Wall -O0 -g  -o exp2 exp2.o
exp3: exp3.o command.o coder.o
	gcc -Wall -O0 -g  -o exp3 coder.o command.o exp3.o
exp1.o: exp1.c
	gcc -Wall -O0 -g  -c -o exp1.o exp1.c
exp2.o: exp2.c
	gcc -Wall -O0 -g  -c -o exp2.o exp2.c
exp3.o: exp3.c
	gcc -Wall -O0 -g  -c -o exp3.o exp3.c
command.o: command.c
	gcc -Wall -O0 -g  -c -o command.o command.c
coder.o: coder.c
	gcc -Wall -O0 -g -c -o coder.o coder.c

clean: 
	rm *.o
