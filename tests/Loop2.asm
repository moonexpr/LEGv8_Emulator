ADDI X20, XZR, #129
ADDI X19, XZR, #0

loop:
    SUB     X18, X20, X19
    PRNT    X18

    ADDI    X19, X19, #1
    SUBS    X18, X20, X19
    CBNZ    X18, loop
    


