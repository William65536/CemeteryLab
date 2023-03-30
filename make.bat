@ECHO OFF

SET CC=clang
SET FLAGS=-Wall -Wextra -Wpedantic -Werror -std=c11
SET SRC=Project\src

:: Make sure that linking the math library is correct

%CC% -c %FLAGS% -o Project\main.o Project\main.c
%CC% -o Project\main Project\main.o -lm

DEL Project\*.o

Project\main

ECHO: && ECHO: && ECHO:

IF ERRORLEVEL 1 (
    ECHO PROGRAM ERROR %ERRORLEVEL%
) ELSE IF ERRORLEVEL 0 (
    ECHO SUCCESS 0
) ELSE (
    ECHO SYSTEM ERROR %ERRORLEVEL%
)