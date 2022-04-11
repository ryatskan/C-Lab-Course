cmake_minimum_required(VERSION 3.21)
project(assemblerProject)

set(CMAKE_CXX_STANDARD 14)

add_executable(assemblerProject main.c main.c preAssemblerRun.c preAssemblerRun.h global.h beautifyLines.c fileFunctions.c fileFunctions.c fileFunctions.h beautifyLines.h utils.c utils.h firstPass.c firstPass.h lineToSentence.c lineToSentence.h secondPass.c secondPass.h directives.c directives.h commands.c commands.h)
