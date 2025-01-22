# Simplex Language

Simplex is a hybrid interpreted scripting language that focuses on mathematics and scientific calculations.
It offers built-in matrix operations (multiplication, determinants, etc.)
as well as several scientific and fundamental constants.

It is strictly typed like languages such C or Java, but has a similar syntax to python with type hinting.


> [!NOTE]
> This project is currently in-progress and is not yet fully functional.  This banner will be removed as the project moves closer to completion.

## Implementation

Simplex is a hybrid interpreted language that was originally written in Java.  I have since made the transition to C++ for better performance and the ability to directly compile the interpreter as an executable.

The source code is first processed into tokens using the Lexed.  It is then built into an abstract syntax tree using an interpreter class.  Finally, the code is interpreted and executed using an executor class.  The language is structured in a way so that the source code can be frozen at any of the steps during its processing as a viewable intermediate representation.  This allows for the language to be an interactive tool for teaching about language processing.

## Execution

After a program has been built into its data structure, it is ready for execution. By the time of execution, all variables and functions have been loaded into memory with their initial values, so execution begins at the beginning and goes line by line.
As each line is hit, any expressions in that line are resolved to their real values, reading the index of an array for example, and functions have the real values or their arguments given to them for their execution.

## Scientific Focus

One of the other notable features of Simplex is its concentration on making certain physics or mathematical operations more straightforward. The language comes with a wide array of built-in mathematical and physical constants and functions. These include constants like *PI8 or the speed of light (*c*), or functions like *Sine*.

Additionally, Simplex options for array manipulation that are not present in many other languages. This comes from the fact that each array is treated as a matrix. This allows for adding any N-Dimensional arrays together, which sum just as arrays would in linear algebra. Integer of Double arrays can also be multiplied together to get the dot or cross product of the two matrices. This becomes even more useful when used with the built-in determinate and inverse functions that can be used on the arrays.