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
    la $sp, -8($fp)
    move $t8, $ra
    lw $ra, -4($fp)
    lw $fp, -8($fp)
    jr $t8

write:
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, _ret
    syscall
    move $v0, $0
    la $sp, -12($fp)
    move $t8, $ra
    lw $ra, -4($fp)
    lw $fp, -8($fp)
    jr $t8

fact:
    addi $sp, $fp, 44
    lw $t1, -12($fp)
    sw $t1, 8($fp)
    li $t1, 1
    sw $t1, 12($fp)
    lw $t1, 8($fp)
    lw $t2, 12($fp)
    ble $t1, $t2, label1
    j label2
label1:
    li $t1, 1
    sw $t1, 16($fp)
    lw $v0, 16($fp)
    la $sp, -12($fp)
    move $t8, $ra
    lw $ra, -4($fp)
    lw $fp, -8($fp)
    jr $t8
    j label3
label2:
    lw $t1, -12($fp)
    sw $t1, 20($fp)
    li $t1, 1
    sw $t1, 24($fp)
    lw $t2, 20($fp)
    lw $t3, 24($fp)
    sub $t1, $t2, $t3
    sw $t1, 28($fp)
    lw $t1, 28($fp)
    sw $t1, 0($sp)
    sw $fp, 4($sp)
    sw $ra, 8($sp)
    la $fp, 12($sp)
    jal fact
    sw $v0, 32($fp)
    lw $t1, -12($fp)
    sw $t1, 36($fp)
    lw $t2, 32($fp)
    lw $t3, 36($fp)
    mul $t1, $t2, $t3
    sw $t1, 40($fp)
    lw $v0, 40($fp)
    la $sp, -12($fp)
    move $t8, $ra
    lw $ra, -4($fp)
    lw $fp, -8($fp)
    jr $t8
label3:

main:
    move $fp, $sp
    addi $sp, $fp, 36
    sw $fp, 0($sp)
    sw $ra, 4($sp)
    la $fp, 8($sp)
    jal read
    sw $v0, 4($fp)
    lw $t1, 4($fp)
    sw $t1, 8($fp)
    lw $t1, 8($fp)
    sw $t1, 12($fp)
    lw $t1, 8($fp)
    sw $t1, 16($fp)
    lw $t1, 16($fp)
    sw $t1, 0($sp)
    sw $fp, 4($sp)
    sw $ra, 8($sp)
    la $fp, 12($sp)
    jal fact
    sw $v0, 20($fp)
    lw $t1, 20($fp)
    sw $t1, 24($fp)
    lw $t1, 24($fp)
    sw $t1, 12($fp)
    lw $t1, 24($fp)
    sw $t1, 28($fp)
    lw $a0, 28($fp)
    sw $fp, 0($sp)
    sw $ra, 4($sp)
    la $fp, 8($sp)
    jal write
    li $t1, 0
    sw $t1, 12($fp)
    li $t1, 0
    sw $t1, 32($fp)
    lw $v0, 32($fp)
    jr $ra
