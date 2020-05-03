ADDI X20, XZR, #4096
ADDI X19, XZR, #0

loop:
    SUBI    SP, SP, #8
    STUR    X19, [SP, #0]
    SUBS    X19, X20, X19
    PRNT    X19
    LDUR    X19, [SP, #0]
    ADDI    SP, SP, #8
    ADDI    X19, X19, #1
    SUBS    X18, X20, X19
    CBNZ    X18, loop
    


