    ADDI X17, XZR, #0 // keep (previous) 0 in X17 for further use
    ADDI X18, XZR, #1  // keep (Current) 1 in X18 for further use
    ADDI X9, XZR, #0  // Assuming i = 0 is in register X9
fibo: 
    SUBI SP, SP, #24 // Adjust stack pointer for 3 items
    STUR LR, [SP, #16] // save the return address
    STUR X17, [SP, #8] //save content of X17 on the stack
    STUR X18, [SP, #0] //save content of X18 on the stack
    SUBS   X10, X9, X19 // test for i==n
    CBNZ X10, L1 // If i not equal to n, go to L1
    ADDI X6, XZR, #0 // keep 0 on X6    
    ADDI X5, XZR, #1 // keep 1 on X5 
    ADDI X2, X9, #1 //X9 increased by 1 for further use
    STUR X6, [X20,#0] //store 0 in the array
    STUR X5, [X20, #8] //store 1 in the array  
    ADDI SP, SP, #24 // pop 3 items from the stack
    BR LR // return to the caller 
L1: 
    ADD X16, X17, X18 // Next_Num = previous + Current
    ADD X17, XZR, X18 // Previous = Current
    ADD X18, XZR, X16 // Current= Next_Num
    ADDI X9, X9, #1 // i++
    BL fibo // call fibo
    LDUR X18, [SP, #0] // return from BL; restore previous
    LDUR X17, [SP, #8] // restore current
    LDUR LR, [SP, #16] // restore the return address
    ADDI SP, SP, #24 // adjust stack pointer to pop 3 items
    ADD X7, X18, X17 // keep (previous + current) value on register X7 
    LSL X2, X2, #3 // Multiplying by 8 for offset    
    ADD X12, X20, X2 // address of the array increase by 8
    STUR X7, [X12, #0] // store (previous + current) value on the array
    SUBI X2, X2, #1 // X9 decreased by 1 
    BR LR // return                           

