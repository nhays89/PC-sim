@echo off
 
set gtk_ver=gtk+-3.0
pkg-config %gtk_ver% --cflags --libs gmodule-2.0>tmp.txt
set /p pkg-info= <tmp.txt
del tmp.txt
 
rem echo %pkg-info%
cd src
gcc simulator/main.c simulator/simulator.c simulator/menubar.c simulator/registerlist.c simulator/instructionlist.c simulator/helper.c model/alu.c model/controlunit.c model/memory.c model/regfile.c -o main.exe -Wall %pkg-info%
main.exe
cd ..
