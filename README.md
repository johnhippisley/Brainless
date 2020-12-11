# Brainless
A simple compiler from an assembly-like addressable language to Brainfuck.<br>
<i>Note</i>: in no way is this compiler optimised (actually, it produces <i>extremley</i> slow programs).

## Usage
<pre>
./brainless <source-file> [-V] 
-V : Produce verbose output
</pre>

## Memory Layout:
Six general purpose registers <i>(R0-R5)</i><br>
Eight special registers used as temporary storage for use by instructions <i>(T0-T8)</i><br>
Twelve extended registers used by internal macros <i>(E0-E12)</i><br>
Eight binary registers used for program flow <i>(B0-B7) </i><br>
Infinite addressable space <i>(D0, D1, ...)</i>, can also be addressed by value with <i>READ</i> and <i>WRITE</i>

#### Diagram: 
<pre>[T8]...[T0][R5]...[R0][HC][B0]...[B7][E0]...[E12][DS][I0][I1][MP](DATA)...</pre>
<i>HC</i> ("home cell", offset of 15) and <i>DS</i> ("data start", offset of 36) are always 0.<br>
<i>I0</i> and <i>I1</i> are always set to the same value.

## Instruction List: 		
Capital arguments can be a location or value, context dependant.<br>
Lowercase arguments must be a value.<br>
Arguments encased in <i>()</i> are optional.<br>

<b>Keywords</b>:
<pre>
<b>Name</b>			<b>Description</b>	
DEBUG			Inserts '!' into output
BF X 			Inserts X into output
MOV X, Y		X = Y
SWAP X, Y		Swaps X and Y
SHIFTR X		Shifts all cells until a zero value to the right starting at X, @(X) == 0
READ X, Y		Reads D0 + Y into X
WRITE X, Y		Writes Y into D0 + X
NOT X			X = !X
AND X, Y		X = X & Y
OR X, Y			X = X || Y
INC X, (y)		X = X + y (or 1, if no y)
DEC X, (y)		X = X - y (or 1, if no y)
ADD X, Y		X = X + Y
SUB X, Y		X = X - Y
MUL X, Y		X = X * Y
DIV X, Y		X = X / Y (integer division)
PUTCHAR	X		Puts X into STDOUT
PUTDEC X		Prints decimal-string of 8-bit value in X
PUTSTR X		Puts null-terminated string at X + 1 into STDOUT, @(X) == 0 
GETCHAR	X		Sets X to character from STDIN	
CMP X, Y		X = (X == Y)	
IF X			Executes up to matching ENDIF if X > 0
ENDIF			End of 'IF' block
WHILE X			Loops up to matching ENDWHILE while X > 0
ENDWHILE		End of 'WHILE' block
DATA X, "..."		Puts ASCII values of string into memory starting at X
DATA X, Y, Z...		Puts values Y, Z, ... into memory starting at X
PRINT "..."		Resolves to series of PUTCHAR instructions. '\n' is resolved to
			10 (ASCII newline), when not escaped with a preceeding '\'
PRINTLN "..."		Resolved to PRINT "..." + PUTCHAR 10 (newline)
</pre>

Here are the keywords seperated by category:<br>
<b>Native instructions</b>:
<pre>
<b>Name</b>			<b>Description</b>	
DEBUG			Inserts '!' into output
BF X 			Inserts X into output
MOV X, Y		X = Y
SWAP X, Y		Swaps X and Y
SHIFTR X		Shifts all cells until a zero value to the right starting at X, @(X) == 0
WRITE_D			Writes MP into D0 + I0
READ_D			Reads D0 + I0 into MP
NOT X			X = !X
AND X, Y		X = X & Y
INC X, (y)		X = X + y (or 1, if no y)
DEC X, (y)		X = X - y (or 1, if no y)
ADD X, Y		X = X + Y
SUB X, Y		X = X - Y
MUL X, Y		X = X * Y
PUTCHAR	X		Puts X into STDOUT
PUTSTR X		Puts null-terminated string at X + 1 into STDOUT, @(X) == 0 
GETCHAR	X		Sets X to character from STDIN	
CMP X, Y		X = (X == Y)	
WHILE X			Loops up to matching ENDWHILE while X > 0
ENDWHILE X		End of 'WHILE' block
IF_D X			Executes up to matching ENDIF_D if X > 0 
ENDIF_D X		End of 'IF' block, destroys value at X
</pre>

<b>Internal macro instructions</b>:
<pre>
<b>Name</b>			<b>Description</b>
IF_0...7 X		Puts X into binary register, thus not destroying the value
ENDIF_0...7 X		End of 'IF' block
OR X, Y			X = X || Y
DIV X, Y		X = X / Y (integer division)
PUTDEC X		Prints decimal-string of 8-bit value in X
READ X, Y		Reads D0 + Y into X
WRITE X, Y		Writes Y into D0 + X
</pre>

<b>Special macros</b>:
<pre>
<b>Name</b>			<b>Description</b>
DATA X, "..."		Puts ASCII values of string into memory starting at X
DATA X, Y, Z...		Puts values Y, Z, ... into memory starting at X
PRINT "..."		Resolves to series of PUTCHAR instructions. '\n' is resolved to
			10 (ASCII newline), when not escaped with a preceeding '\'
PRINTLN "..."		Resolved to PRINT "..." + PUTCHAR 10 (newline)
</pre>

## How does the compiler work? 
The compiler does the following: 
1. Read program from source
2. Format program to make it easily parsed
3. Resolve special macros 
4. Automatically resolve <i>IF</i> and <i>WHILE</i> blocks (i.e. add matching argument to <i>ENDWHILE</i>, resolve <i>IF</i> to <i>IF_0...7</i>)
5. Parse into a program data structure
6. Resolve internal macros
7. Resolve identifiers (i.e. replace <i>HC</i> with <i>@(15)</i>, etc.)
8. Generate pseudo-BF (an intermediate language in which the native instructions are implemented in)
9. Compile to Brainfuck

<i>Psueso-BF</i> is a shorthand version of Brainfuck with the following mnemonics:
<pre>
<b>Syntax</b>		<b>Implementation</b>
@(x)		Move data pointer by to position X
x(y)		Repeat the character 'x' y times
</pre>

## Examples
<b>Hello, world!</b>
<pre>
PRINTLN "Hello, world!"
</pre>
<i>or</i>
<pre>
DATA D0, "\0Hello, world!\n\0"
PUTSTR D0
</pre>
<b>Reverse string</b>
<pre>
# Read inputted string into memory until newline
MOV R0, 0	# Holds current character
MOV R1, 10	# Newline
MOV R2, 1	# Loop flag
MOV R3, 0	# Write index
WHILE R2
	GETCHAR R0
	WRITE R3, R0
	INC R3
	MOV R2, R0
	CMP R2, R1
	NOT R2
ENDWHILE

PRINT "Reversed: "
# Print reversed output
DEC R3
WHILE R3
	DEC R3
	READ R0, R3
	PUTCHAR R0
ENDWHILE
PUTCHAR 10
</pre>
<b>99 bottles of beer on the wall</b>
<pre>
MOV R0, 99
WHILE R0
	PUTDEC R0
	PRINT " bottles of beer on the wall, "
	PUTDEC R0
	PRINT " bottles of beer.\nTake one down, pass it around, "
	DEC R0
	PUTDEC R0
	PRINTLN " bottles of beer on the wall."
ENDWHILE
</pre>
<b>Print first 14 Fibbonaci numbers</b>
<pre>
MOV R0, 0
MOV R1, 1
PUTDEC R0
PUTCHAR 10
PUTDEC R1
PUTCHAR 10
 
MOV R2, 12
WHILE R2
	SWAP R0, R1
	ADD R1, R0
	PUTDEC R1
	PUTCHAR 10	
	DEC R2
ENDWHILE
</pre>

## TODO
* Add instructions compatible with data types over 8 bits<br>
* Add <i>GETDEC</i> instruction for easy input<br>
* Optimization (!!)
