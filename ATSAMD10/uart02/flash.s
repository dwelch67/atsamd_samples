
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
.word 0x20001000
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
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr


.thumb_func
.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr

.thumb_func
.globl PUT8
PUT8:
    strb r1,[r0]
    bx lr

.thumb_func
.globl GET8
GET8:
    ldrb r0,[r0]
    bx lr

.thumb_func
.globl dummy
dummy:
    bx lr

