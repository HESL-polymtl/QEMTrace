/*******************************************************************************
 * tracing_mod.S
 *
 * Author  Alexy Torres Aurora Dugo
 * Version 1.0
 *
 * MiniKernel test: Check if tracing modification at runtime works
 ******************************************************************************/
li r4, 0x7000
li r5, 0x1234

addi r4, r4, 0x1000
stw r5, 0(r4)

addi r4, r4, 0x1000
lwz r5, 0(r4)

addi r4, r4, 0x1000
sth r5, 0(r4)

addi r4, r4, 0x1000
lhz r5, 0(r4)

/* Disables data only */
.long 0xFFFFFFF3

addi r4, r4, 0x1000
stb r5, 0(r4)

addi r4, r4, 0x1000
lbz r5, 0(r4)

/* Disables instruction only */
.long 0xFFFFFFF5

addi r4, r4, 0x1000
lbz r5, 0(r4)

addi r4, r4, 0x1000
lbz r5, 0(r4)