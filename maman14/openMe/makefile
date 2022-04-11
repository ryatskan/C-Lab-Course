assemblerProject: main.o commands.o directives.o fileFunctions.o lineToSentence.o preAssemblerRun.o firstPass.o secondPass.o beautifyLines.o utils.o
	gcc -Wall -ansi -pedantic -g main.o commands.o directives.o fileFunctions.o lineToSentence.o beautifyLines.o utils.o preAssemblerRun.o secondPass.o firstPass.o -o assembler -lm
main.o: main.c
	gcc -c -Wall -ansi -pedantic main.c -o main.o
	
commands.o: commands.c commands.h
	gcc -c -Wall -ansi -pedantic commands.c -o commands.o -lm
	
directives.o: directives.c directives.h
	gcc -c -Wall -ansi -pedantic directives.c -o directives.o
	
preAssemblerRun.o: preAssemblerRun.c preAssemblerRun.h
	gcc -c -Wall -ansi -pedantic preAssemblerRun.c -o preAssemblerRun.o
	
firstPass.o: firstPass.c firstPass.h
	gcc -c -Wall -ansi -pedantic firstPass.c -o firstPass.o
	
secondPass.o: secondPass.c secondPass.h
	gcc -c -Wall -ansi -Werror -pedantic secondPass.c -o secondPass.o
	
lineToSentence.o: lineToSentence.c lineToSentence.h
	gcc -c -Wall -ansi -pedantic lineToSentence.c -o lineToSentence.o
	
utils.o: utils.c utils.h
	gcc -c -Wall -ansi -pedantic utils.c -o utils.o
	
beautifyLInes.o: beautifyLines.c beautifyLines.h
	gcc -c -Wall -ansi -pedantic utils.c -o beautifyLInes.o
fileFunctions.o: fileFunctions.c fileFunctions.h
	gcc -c -Wall -ansi -pedantic fileFunctions.c -o fileFunctions.o
