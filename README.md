# RPNcalc

## Description
RPNcalc is a command-line calculator that uses Reverse Polish Notation (RPN) for input. It supports basic arithmetic operations, stack manipulation, and variable storage. The calculator is designed to be efficient and user-friendly, providing a simple interface for performing calculations.

### Available Operations
- Addition (+, `add`)
- Subtraction (-, `sub`)
- Multiplication (*, `mul`)
- Division (/, `div`)
- Modulus (`mod`)
- Exponentiation (`pow`)
- Negation (`neg`)
- Stack Manipulation:
  - Clear Stack (`clear`)
  - Duplicate Top Element (`dup`)
  - Swap Top Two Elements (`swap`)
  - Drop Top Element (`drop`)
- Variable Storage:
  - Store Variable (`=name`)
  - Retrieve Variable (`name`)
- Trigonometric Functions:
  - Sine (`sin`)
  - Cosine (`cos`)
  - Tangent (`tan`)
  - Inverse Sine (`asin`)
  - Inverse Cosine (`acos`)
  - Inverse Tangent (`atan`)
  - Hyperbolic Sine (`sinh`)
  - Hyperbolic Cosine (`cosh`)
  - Hyperbolic Tangent (`tanh`)
  - Inverse Hyperbolic Sine (`asinh`)
  - Inverse Hyperbolic Cosine (`acosh`)
  - Inverse Hyperbolic Tangent (`atanh`)
  - Arctangent of Two Variables (`atan2`)
- Logarithmic Functions:
  - Natural Logarithm (`ln`)
  - Base-10 Logarithm (`log`)
  - Base-2 Logarithm (`log2`)
  - Exponential Function (`exp`)
  - Power Function (`pow`)
  - Square Root (`sqrt`)
  - Cube Root (`cbrt`)
  - Absolute Value (`abs`)
  - Factorial (`fact`)
  - Greatest Common Divisor (`gcd`)
  - Least Common Multiple (`lcm`)
- Miscellaneous Functions:
  - Floor (`floor`)
  - Ceiling (`ceil`)
  - Round (`round`)
  - Truncate (`trunc`)
  - Random Number Generation (`rand`)
- Combinatorial Functions:
  - Permutations (`npr`)
  - Combinations (`ncr`)

### Usage
To use RPNcalc, simply run the executable and enter your calculations in RPN format.

Example:
```
5 3 +
```
This will output `8`, as it adds `5` and `3`.

### Errors
RPNcalc provides error codes for various issues.
| Error Code | Description |
|------------|-------------|
| `E01`      | Unknown operator |
| `E02`      | Invalid arguments |
| `E03`      | Stack underflow |
| `E04`      | Stack overflow |
| `E05`      | Divide by zero |
| `E06`      | Internal error |
| `E07`      | Invalid variable name |
| `E08`      | Variable not writeable |
