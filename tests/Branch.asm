bar:
    BL      foo
    ADDI    X0, XZR, #0xFF
    BL      foo
    B done

foo:
    PRNT    X0
    BR      LR

done:
