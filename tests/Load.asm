// store number of fib numbers to calculate
ADDI X19, XZR, #33
// iterator variable
ADDI X20, XZR, #0
loop:
SUB X0, X19, X20
CBZ X0, end_loop
ADDI X0, X20, #1
BL fib
// print fib(n)
PRNT X0
ADDI X20, X20, #1
B loop
end_loop:

B end

fib:
// X0 - n
// check if n < 2
ADDI X1, XZR, #2
SUBS X1, X0, X1
B.LT end_fib
DUMP
// save link register
SUBI SP, SP, #16
STUR LR, [SP, #8]
// save n on stack
STUR X0, [SP, #0]
// fib(n - 2)
SUBI X0, X0, #2
BL fib
// load n from stack
LDUR X1, [SP, #0]
// save fib(n - 2) on stack
STUR X0, [SP, #0]
// fib(n - 1)
SUBI X0, X1, #1
BL fib
LDUR X1, [SP, #0]
// add fib(n - 2) and fib(n - 1)
ADD X0, X1, X0
ADDI SP, SP, #8
// restore link register
LDUR LR, [SP, #0]
ADDI SP, SP, #8
end_fib:
BR LR

end:
