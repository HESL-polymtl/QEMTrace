; simple_address_tracing.asm
;
; Author  Alexy Torres Aurora Dugo
; Version 1.0
;
; MiniKernel test: Simple trace tests to check if Qemu is correctly getting all
; the wnated information about accesses.

mov eax, 0x300000
mov ebx, 0xDEADBEEF

; D ST | V 0x00300000 | P 0x0020c000 Core | Time 0, Flags 0x0000020B
mov [eax], ebx

; D LD | V 0x00300000 | P 0x0020c000 Core | Time 0, Flags 0x0000020A
mov ebx, [eax]

; D ST | V 0x00300000 | P 0x0020c000 Core | Time 0, Flags 0x0000010B
mov [eax], bx

; D LD | V 0x00300000 | P 0x0020c000 Core | Time 0, Flags 0x0000010A
mov bx, [eax]

; D ST | V 0x00300000 | P 0x0020c000 Core | Time 0, Flags 0x0000000B
mov [eax], bl

; D LD | V 0x00300000 | P 0x0020c000 Core | Time 0, Flags 0x0000000A
mov bl, [eax]
