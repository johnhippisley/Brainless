# Brainlesss
A simple, extensive (will be), compiler from an assembly-like language to Brainfuck.<br>
<i>Note</i>: in no way is this compiler optimised (actually, it produces <b>extremley</b> slow programs).<br>
Written by John Hippisley, 2020

## Usage
<pre>
./brainless <source-file> [-V] 
-V : Produce verbose output
</pre>

## Memory Layout:
Six general purpose registers (R0-R5)
Eight special registers used as temporary storage for use by instructions (T0-T8)
Twelve extended registers used by internal macros (E0-E12)
Eight binary registers used for program flow (B0-B7) 

Diagram: 
[T8]...[T0][R5]...[R0][HC][B0]...[B7][E0]...[E12][DS][D0][D1]...  		
HC ("home cell", offset of 15) and DS ("data start", offset of 36) are always 0.

## Instruction List: 		
Capital arguments could be a location or value, context dependant.
Lowercase arguments must be a value.
Arguments encased in () are optional.

Native instructions:
<pre>
-Name-			-Description-	
DEBUG			Inserts '!' into output
BF X 			Inserts X into output
MOV	X, Y		X = Y
SWAP X, Y		Swaps X and Y
SHIFTR X		Shifts all cells until a zero value to the right starting at X, @(X) == 0
NOT	X		X = !X
AND X, Y		X = X & Y
INC	X, (y)		X = X + y (or 1, if no y)
DEC	x, (y)		X = X + y (or 1, if no y)
ADD X, Y		X = X + Y
SUB X, Y		X = X - Y
MUL X, Y		X = X * Y
PUTCHAR	X		Puts X into STDOUT
PUTSTR X		Puts null-terminated string at X + 1 into STDOUT, @(X) == 0 
GETCHAR	X		Sets X to character from STDIN	
CMP	X, Y		X = (X == Y)	
WHILE X			Loops up to matching ENDWHILE while X > 0
ENDWHILE X		End of 'WHILE' block
IF_D X			Executes up to matching ENDIF_D if X > 0 
ENDIF_D X		End of 'IF' block, destroys value at X
</pre>

Internal macro instructions:
<pre>
-Name-			-Description-
IF_0...7 X		Puts X into binary register, thus not destroying the value
ENDIF_0...7 X		End of 'IF' block
OR X, Y			X = X || Y
DIV X, Y		X = X / Y (integer division)
PUTDEC X		Prints decimal-string of 8-bit value in X
</pre>

Special macros:
<pre>
-Name-			-Description-
PRINT "..."		Resolves to series of PUTCHAR instructions. '\n' is resolved to
				10 (ASCII newline), when not escaped with a preceeding '\'
PRINTLN "..."		Resolved to PRINT "..." + PUTCHAR 10 (newline)
</pre>

## How does the compiler work? 
The compiler does the following: 
1. Read program from source
2. Format program to make it easily parsed
3. Resolve special macros 
4. Automatically resolve IF and WHILE blocks (i.e. add matching argument to ENDWHILE, resolve IF to IF_0...7)
5. Parse into a program data structure
6. Resolve internal macros
7. Resolve identifiers (i.e. replace HC with @(15), etc.)
8. Generate pseudo-BF (an intermediate language in which the native instructions are implemented in)
9. Compile to Brainfuck

Psueso-BF is a shorthand version of Brainfuck with the following mnemonics:
<pre>
-Syntax-		-Implementation-
@(x)			Move data pointer by to position X
x(y)			Repeat the character 'x' y times
</pre>
