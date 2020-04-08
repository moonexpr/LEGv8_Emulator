loop:
	ADDI	X0, XZR, #1
	ADDI	X1, XZR, #20
	SUB	X1, X1, X0
	CBNZ	X1, loop

loop2:
	SUBI 	X0, X0, #1000
	B.LO	loop2
	
