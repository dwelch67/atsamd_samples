
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
.word 0x20000200
.word reset
.word hang
.word hang

.word hang
.word hang
.word hang
.word hang

.word hang
.word hang
.word hang
.word hang

.word hang
.word hang
.word hang
.word hang

.thumb_func
reset:
    bl notmain
    b hang
.thumb_func
hang:   b .

.thumb_func
.globl dummy
dummy:
    bx lr
