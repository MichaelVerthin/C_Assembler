## $\color{rgb(255,127,39)}\large{\textsf{ANCI C Assembly Compliler}}$

This project presents a robust pseudo assembly language assembler, focusing on the translation component.<br />
Developed for a C class course at the Open University of Israel, it forms the core translation engine within a comprehensive assembler software suite
> [!TIP]
> 1. <strong>Build:</strong> Use `make` to create the assembler program (`myprog`)<br />
> 2. <strong>Assemble:</strong> Run `./myprog` followed by your assembly file (e.g., `mycode.as`)

## $\color{rgb(255,127,39)}\large{\textsf{Target Machine Architecture Overview}}$

- <strong>Central Processing Unit (CPU):</strong> Features 8 general-purpose registers (r0-r7) with a word size of 14 bits.
- <strong>Instruction Set Architecture (ISA):</strong> Supports 16 distinct opcodes encompassing fundamental operations like:
  - move (mov)
  - compare (cmp)
  - arithmetic operations (add, sub)
  - logical operations (not, clr)
  - memory access (lea)
  - control flow (jmp, bne)
  - subroutine calls (jsr, rts)
  - and program termination (hlt).
- <strong>Memory:</strong> Comprises 4096 memory locations, each capable of storing 14-bit data.
- <strong>Data Type Support:</strong> The assembler is designed to handle both ASCII characters and integer values. Real number processing is currently not supported.
> [!WARNING]
> <strong>Invalid Register Usage:</strong> Register usage outside the valid range (r0-r7) will cause errors.<br />
> <strong>Data Type Limitations:</strong> The assembler currently only supports integer and ASCII characters. Real numbers will cause errors

## $\color{rgb(255,127,39)}\large{\textsf{Instruction Word Structure}}$

The target machine utilizes a 14-bit instruction word format for efficient encoding:
```
+----------+--------+--------+-------------+-------+
|  10-13   |  6-9   |  4-5   |     2-3     |  0-1  |
| Not Used | Opcode | source | destination | A.R.E |
+----------+--------+--------+-------------+-------+
```
- <strong>Addressing Mode Extension (A.R.E) (0-1 bit):</strong> Currently unused, potentially for future addressing mode support.
- <strong>Destination Register (2-3 bits):</strong> Specifies the destination register operand (if applicable to the instruction).
- <strong>Source Register (4-5 bits):</strong> Specifies the source register operand (if applicable to the instruction).
- <strong>Opcode (6-9 bits):</strong> Identifies the specific operation to be performed (e.g., mov, cmp, add).
- <strong>Not Used (10-13 bits):</strong> Reserved for future instruction set expansion.
> [!IMPORTANT]
> The assembler uses a 14-bit instruction word format for efficient encoding. Understanding this structure is crucial for writing correct assembly instructions

## $\color{rgb(255,127,39)}\large{\textsf{Compiler Compatibility}}$

<div align="left">
The project is designed to function seamlessly with an ANSI standard C compiler, ensuring broad compatibility with modern development environments
</div>

<br />
> [!NOTE]
> If you encounter any compiler-related issues, double-check that you're using an ANSI standard C compiler
