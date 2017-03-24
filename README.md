# PC-Sim

This project is an abstraction of a computer’s data path design using object-oriented programming in C with supporting GIMP Toolkit (GTK+ 3) libraries to simulate the fetch-decode-execute cycle of an assembled program using the LC-2200 Instruction Set Architecture (ISA). The LC-2200 ISA supports RISC-style instructions with four types of addressing modes (R-type, I-type, J-type, and O-type) and eight different instructions: add, nand, addi, lw, sw, beq, jalr, and nop. The register-oriented 32-bit single-bus data path design contains five main components to support the CPU: memory, ALU, register file, program counter (PC), and instruction register (IR). These components serve as vehicles to carry out the simple instructions specified in the LC-2200 ISA to cater the needs of any high-level language. 

**External Resources**
<ul>
<li><a href="https://developer.gnome.org/gtk3/stable/index.html">GIMP Toolkit</a></li>
<li><a href="http://www.cc.gatech.edu/~rama/CS2200-External/projects/p2/LC-2200-32.html">LC-2200</a></li>
</ul>

**What do I need?**
<p> Be sure to set environment variable paths for these programs to link libraries (windows) </p>

<ul>
<li><a href="https://sourceforge.net/projects/mingw/">MinGW</a></li>
<li><a href="https://sourceforge.net/projects/pkgconfiglite/">pkg-config-lite</a></li>
<li><a href="https://sourceforge.net/projects/gtk-win/?source=directory">GTK+</a></li>
</ul>

**Environment Variables** 
```
C:\MinGW\bin
C:\MinGW\gtk\include\gtk-3.0\gdk
C:\MinGW\gtk\bin
C:\MinGW\pkg-config-lite\bin
C:\MinGW\gtk\lib\gmodule-2.0

```

**How to operate?**

  1. (if modifying src only) download MinGW (gcc compilier) and pkg-config-lite (helper tool used for easy compiling -extract folder inside C:\MinGW as "pkg-config-lite"), install GTK (place inside C:\MinGW name folder as "gtk") edit system environment variables stated above. 
  2. clone repo.
  3. run gui.exe in src/assembler.
  4. copy text from official_test1.txt into assembler gui text area (an example assembly program) or load an assembly file from your hard drive.
  5. click 'assemble' menu_item.
  6. save assembly file to desktop or a location on a preferred drive.
  7. launch gui.exe in src/simulator
  8. open file menu, click 'load' menu item.
  9. navigate to assembled program and select it.
  10. press 'F5' to execute program instructions line-by-line
  11. repeat process from step 4, except now right your own assembly program using the <a href="http://www.cc.gatech.edu/~rama/CS2200-External/projects/p2/LC-2200-32.html">LC-2200</a> ISA.
 

**Batch File**
<p> Links C modules to accomodate easy compile line execution (only needed if modifying source code)</p>

```
 
@echo off
 
set gtk_ver=gtk+-3.0
pkg-config %gtk_ver% --cflags --libs gmodule-2.0>tmp.txt
set /p pkg-info= <tmp.txt
del tmp.txt
 
rem echo %pkg-info%
cd src
gcc simulator/main.c simulator/simulator.c simulator/menubar.c simulator/registerlist.c simulator/instructionlist.c simulator/helper.c model/alu.c model/controlunit.c model/memory.c model/regfile.c -o simulator/gui.exe -Wall %pkg-info%
cd simulator
gui.exe
cd ..

```
***Assembler***

![asm](https://github.com/nhays89/PC-sim/blob/master/assembler.png)

***Simulator***

![sim](https://github.com/nhays89/PC-sim/blob/master/simulator.png)
