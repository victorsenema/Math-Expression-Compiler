# Simple Expression Compiler

This project implements a simple compiler/interpreter for a small expression-based language.
It supports variable assignment, arithmetic expressions, and basic mathematical functions,
using a recursive descent (LL(1)) parser with direct evaluation (no AST).

---

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

### 2) Print

**Syntax**
```txt
print(expression);
```

**Examples**
```txt
print(x);
print(2 + 3 * 4);
print((x + y) / 2);
```

### 3) Expressions
An expression may contain:
**Numbers**
```txt
2
3.14
42
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
