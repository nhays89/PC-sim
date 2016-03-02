@echo off
 
set gtk_ver=gtk+-3.0
pkg-config %gtk_ver% --cflags --libs >tmp.txt
set /p pkg-info= <tmp.txt
del tmp.txt
 
rem echo %pkg-info%
cd src
gcc menubar.c alu.c controlunit.c memory.c regfile.c -o main.exe -Wall %pkg-info%
main.exe
cd ..