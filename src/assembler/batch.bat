@echo off
 
set gtk_ver=gtk+-3.0
pkg-config %gtk_ver% --cflags --libs >tmp.txt
set /p pkg-info= <tmp.txt
del tmp.txt
 
rem echo %pkg-info%
gcc assembler.c assemblerGUI.c -o gui.exe -Wall %pkg-info%

