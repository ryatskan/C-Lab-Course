complex: mycomp.o complex.o readNewLineHelper.o runCommandAndFindErrors.o findErrorsPreReading.o
	gcc -ansi -Wall -pedantic -g mycomp.o complex.o readNewLineHelper.o runCommandAndFindErrors.o findErrorsPreReading.o -o complex -lm
mycomp.o: mycomp.c complex.h
	gcc -c -Wall -ansi -pedantic mycomp.c -o mycomp.o
complex.o: complex.c complex.h
	gcc -c -Wall -ansi -pedantic complex.c -o complex.o -lm
readNewLineHelper.o: readNewLineHelper.c complex.h
	gcc -c -Wall -ansi -pedantic readNewLineHelper.c -o readNewLineHelper.o
runCommandAndFindErrors.o: runCommandAndFindErrors.c complex.h
	gcc -c -Wall -ansi -pedantic runCommandAndFindErrors.c -o runCommandAndFindErrors.o
findErrorsPreReading.o: findErrorsPreReading.c complex.h
	gcc -c -Wall -ansi -pedantic findErrorsPreReading.c -o findErrorsPreReading.o

