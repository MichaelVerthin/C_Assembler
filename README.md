## $\color{rgb(255,127,39)}\large{\textsf{ANCI C Assembly Compliler}}$

<div align="left">
This project presents a robust pseudo assembly language assembler, focusing on the translation component. Developed for a C class course at the Open University of Israel, it forms the core translation engine within a comprehensive assembler software suite
</div>

## $\color{rgb(255,127,39)}\large{\textsf{Target Machine Architecture Overview}}$

- <strong>Central Processing Unit (CPU):</strong> Features 8 general-purpose registers (r0-r7) with a word size of 14 bits.
+ <strong>Instruction Set Architecture (ISA):</strong> Supports 16 distinct opcodes encompassing fundamental operations like\n move (mov), compare (cmp), arithmetic operations (add, sub), logical operations (not, clr), memory access (lea), control flow (jmp, bne), subroutine calls (jsr, rts), and program termination (stop).
* <strong>Memory:</strong> Comprises 4096 memory locations, each capable of storing 14-bit data.
* <strong>Data Type Support:</strong> The assembler is designed to handle both ASCII characters and integer values. Real number processing is currently not supported.

## $\color{rgb(255,127,39)}\large{\textsf{Instruction Word Structure:}}$

<div align="left">
The target machine utilizes a 14-bit instruction word format for efficient encoding:
+------------+------------+------------+---------------+-------+
|    10-13   |     6-9    |     4-5    |      2-3      |  0-1  |
|  Not Used  |   Opcode   |   source   |  destination  | A.R.E |
+------------+------------+------------+---------------+-------+
**• Not Used (10-13 bits): Reserved for future instruction set expansion.
**• Opcode (6-9 bits): Identifies the specific operation to be performed (e.g., mov, cmp, add).
**• Source Register (4-5 bits): Specifies the source register operand (if applicable to the instruction).
**• Destination Register (2-3 bits): Specifies the destination register operand (if applicable to the instruction).
**• Addressing Mode Extension (A.R.E) (0-1 bit): Currently unused, potentially for future addressing mode support.
</div>
