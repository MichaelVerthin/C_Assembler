<div align="center">

![Profile Views](https://komarev.com/ghpvc/?username=MichaelVerthin&style=for-the-badge&color=FF6E32&base=1230)

</div>

![image](https://github.com/MichaelVerthin/MichaelVerthin/assets/86958340/7f82b9c4-8e74-4c68-a82e-f9eaf32b7004)

<div align="center">

<h1 align="center">Assembler</h1>

<h3 align="center">Pseudo assembly language assembler</h3>

<h2 align="center">Introduction</h2>

<p align="center">This repository is the final project of my C class course in the Open University of Israel.
The project is merely the translator part of the whole Assembler software.</p>

<h2 align="center">The Machine</h2>

<p align="center">The CPU has 8 registers, each is of size of 14 bits.
registers: r0, r1, r2, r3, r4, r5, r6, r7.
It supports 16 types of opcodes:
    - mov  
    - cmp  
    - add  
    - sub  
    - not  
    - clr  
    - lea  
    - inc  
    - dec  
    - jmp  
    - bne  
    - red  
    - prn  
    - jsr  
    - rts  
    - stop  
There are 4096 memory cells of 14 bits each.
It supports only ascii characters and whole numbers, real numbers throws an error.</p>

<h3 align="center">Word architecture</h3>

<p align="center">
    +------------+------------+------------+---------------+-------+ <br>
    |       10-13|         6-9|         4-5|            2-3|    0-1| <br>
    |            |            |            |               |       | <br>
    |  Not Used  |   Opcode	  |   source   |  destination  | A.R.E | <br>
    |            |            |            |               |       | <br>
    +------------+------------+------------+---------------+-------+
</p>

<h4 align="center">Note</h4>

<p align="center">The project is intended to work with ansi standard compiler</p>
