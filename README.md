# Simple Expression Compiler

This project implements a simple compiler/interpreter for a small expression-based language.
It supports variable assignment, arithmetic expressions, and basic mathematical functions,
using a recursive descent (LL(1)) parser with direct evaluation (no AST).

---
## How to compile
Go to the src folder, open the command prompt (or terminal), and run:
```txt
make
```
This will generate the executable if it does not already exist.
Then run the compiler with:
```txt
./compiler.exe
```
The program will start and prompt you to enter the name of the file you want to compile.
Type the name of the file (for example, ExampleCode.txt) and press Enter.

##Writing and executing your own code
To write your own code, go to the Codes To Execute folder and create a .txt file containing your code.

After that, simply run the compiler again and enter the name of your file when prompted.
If the code contains errors, the compiler will display them. Otherwise, it will successfully execute all the expressions and functions defined in the file.
Note: The file name must include the .txt extension.

## Language Supported by the Compiler

The compiler supports two kinds of statements:

### 1) Variable Assignment

**Syntax**
```txt
name = expression;
```
**Examples**
```txt
x = 10;
y = x + 2 * 3;
total = (x + y) ^ 2;
```

### 2) Functions

Functions are called using the following syntax:

**Syntax**
```txt
function_name(expression);
function_name(expression1, expression2);
```

Some functions accept one argument, others accept two arguments.
Passing an incorrect number of arguments results in a error.

**print(expression)**
Prints the evaluated result of an expression to the standard output.
```txt
print(x);
print(2 + 3 * 4);
print((x + y) / 2);
```

**log(a, b)**
Computes the logarithm of a with base b.
```txt
log(8, 2)   -> 3
log(100, 10) -> 2
```

**max(a, b)**
Returns the maximum of two values.
```txt
max(10, 20) -> 20
max(x, y)
```

**min(a, b)**
Returns the minimum of two values.
```txt
min(10, 20) -> 10
min(x, y)
```

**sin(x)**
Computes the sine of x (in radians).
```txt
sin(0)      -> 0
sin(pi / 2) -> 1
```


**cos(x)**
Computes the cosine of x (in radians).
```txt
cos(0)      -> 1
cos(pi)     -> -1
```

**abs(x)**
Returns the absolute value of x.
```txt
abs(-2) -> 2
abs(x - y)
```


### 3) Expressions
An expression may contain:
**Numbers**
```txt
2
3.14
42
```
There are default number:
```txt
pi
e
```

**Variables**
```txt
x
total
result
```

**Parentheses**
```txt
(1 + 2) * 3
(x + y) ^ 2
```

**Operators (by precedence)**
| Operator | Description    |
| -------: | -------------- |
|      `^` | power          |
|      `*` | multiplication |
|      `/` | division       |
|      `%` | modulo         |
|      `+` | addition       |
|      `-` | subtraction    |

**Examples**
```txt
2 + 3 * 4 -> 14
(2 + 3) * 4 -> 20
2 ^ 3 ^ 2 -> 2^(3^2)
```

**Mathematical Functions**

Currently supported:
```txt
log(a, b)
```

**Examples**
```txt
log(8, 2) -> 3
print(log(100, 10));
```

**Full Program Example**
```txt
x = 2;
y = 3;

z = x + y * 4;
print(z);

print((z ^ 2) % 5);
print(log(8, 2));
```
