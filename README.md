# PC-Sim

This project is an abstraction of a computer’s data path design using object-oriented programming in C with supporting GIMP Toolkit (GTK) libraries to support front-end display. The model is built in accordance with the 32-bit, fixed-length LC-2200 Instruction Set Architecture (ISA) which supports four types of addressing modes (R-type, I-type, J-type, and O-type) and a total of 8 different instructions: ADD, NAND, ADDI, LW, SW, BEQ, JALR, and NOP. The core program files consist of four basic structures that represent the combinational and sequential-logic hardware elements of the data path: Central Processing Unit (CPU), Memory, Arithmetic Logic Unit (ALU), and Register set. The CPU—in conjunction with its supporting structures— will mimic the Finite State Machine (FSM) to orchestrate the fetch, decode and execution cycles of a modern processor. 

**External Resources**
<ul>
<li><a href="https://developer.gnome.org/gtk3/stable/index.html">GIMP Toolkit</a></li>
<li><a href="http://www.cc.gatech.edu/~rama/CS2200-External/projects/p2/LC-2200-32.html">LC-2200</a></li>
</ul>

**Necessary programs for windows**
<p> Be sure to set environment variable paths for these programs to link libraries</p>

<ul>
<li><a href="https://sourceforge.net/projects/mingw/">MinGW</a></li>
<li><a href="https://sourceforge.net/projects/pkgconfiglite/">pkg-config-lite</a></li>

</ul>
**Batch File**
<p> Links c modules to accomodate easy compile line execution</p>
```
@echo off
 
set gtk_ver=gtk+-3.0
pkg-config %gtk_ver% --cflags --libs gmodule-2.0>tmp.txt
set /p pkg-info= <tmp.txt
del tmp.txt
 
rem echo %pkg-info%
cd src
gcc simulator_main.c alu.c controlunit.c memory.c regfile.c -o main.exe -Wall %pkg-info%
main.exe
cd ..

```









