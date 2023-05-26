.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text

read:
    li $v0, 4
    la $a0, _prompt
    syscall
    li $v0, 5
    syscall
    jr $ra

write:
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, _ret
    syscall
    move $v0, $0
    jr $ra

main:
    move $fp, $sp
    jal read
    sw $v0, 4($fp)
    lw $t1, 4($fp)
    sw $t1, 8($fp)
    lw $t1, 8($fp)
    sw $t1, 12($fp)
    jal read
    sw $v0, 16($fp)
    lw $t1, 16($fp)
    sw $t1, 20($fp)
    lw $t1, 20($fp)
    sw $t1, 12($fp)
    lw $t1, 8($fp)
    sw $t1, 24($fp)
    lw $t1, 20($fp)
    sw $t1, 28($fp)
    lw $t2, 24($fp)
    lw $t3, 28($fp)
    add $t1, $t2, $t3
    sw $t1, 32($fp)
    lw $a0, 32($fp)
    jal write
    li $t1, 0
    sw $t1, 12($fp)
    lw $t1, 8($fp)
    sw $t1, 36($fp)
    lw $t1, 20($fp)
    sw $t1, 40($fp)
    lw $t2, 36($fp)
    lw $t3, 40($fp)
    sub $t1, $t2, $t3
    sw $t1, 44($fp)
    lw $a0, 44($fp)
    jal write
    li $t1, 0
    sw $t1, 12($fp)
    lw $t1, 8($fp)
    sw $t1, 48($fp)
    lw $t1, 20($fp)
    sw $t1, 52($fp)
    lw $t2, 48($fp)
    lw $t3, 52($fp)
    mul $t1, $t2, $t3
    sw $t1, 56($fp)
    lw $a0, 56($fp)
    jal write
    li $t1, 0
    sw $t1, 12($fp)
    lw $t1, 8($fp)
    sw $t1, 60($fp)
    lw $t1, 20($fp)
    sw $t1, 64($fp)
    lw $t2, 60($fp)
    lw $t3, 64($fp)
    div $t2, $t3
    mflo $t1
    sw $t1, 68($fp)
    lw $a0, 68($fp)
    jal write
    li $t1, 0
    sw $t1, 12($fp)
    li $t1, 0
    sw $t1, 72($fp)
    lw $v0, 72($fp)
    jr $ra
