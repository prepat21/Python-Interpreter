# Python Interpreter in C++
Includes integer variables, basic expression evaluation, control flow, and functions.

Additionally, the interpreter supports recursion.

To compile run the following in the main directory:
`g++ -std=c++11 *.cpp -o mypython`

Usage:
`./mypython <file.py>`

To compile and test the script, run:
`./test.sh`


## General Structure & Core Functions
The interpreter is separated into three phases: scanning, parsing, and the interpreter runtime.

Regarding handling scope in the interpreter, an Environment class was created. This Environment stores variables and functions declared or initialized in a specific scope. The interpreter has two types of environments, global and the callstack. Everytime a function is called, the interpreter creates a callstack Environment and allocates/gets variables prioritizing there before the global scope.

Extra Credit: The C++ interpreter is equipped to handle single and mutual recursion. For technical evaluation, users are encouraged to implement a pair of functions that engage in mutual recursion. This test will assess the interpreter's stack management and its ability to process deeply nested recursive calls, crucial for evaluating the robustness and computational efficiency of the interpreter in handling complex recursive structures.
