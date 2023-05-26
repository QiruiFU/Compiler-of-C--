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
    sw $v0, 164($fp)
    lw $t1, 164($fp)
    sw $t1, 168($fp)
    lw $t1, 168($fp)
    sw $t1, 172($fp)
    li $t1, 0
    sw $t1, 176($fp)
    li $t1, 2
    sw $t1, 180($fp)
    lw $t2, 180($fp)
    li $t3, 1
    mul $t1, $t2, $t3
    sw $t1, 180($fp)
    lw $t2, 176($fp)
    lw $t3, 180($fp)
    add $t1, $t2, $t3
    sw $t1, 176($fp)
    lw $t2, 176($fp)
    li $t3, 4
    mul $t1, $t2, $t3
    sw $t1, 176($fp)
    la $t2, 4($fp)
    lw $t3, 176($fp)
    add $t1, $t2, $t3
    sw $t1, 176($fp)
    lw $t1, 176($fp)
    sw $t1, 184($fp)
    li $t1, 1
    sw $t1, 188($fp)
    lw $t1, 188($fp)
    lw $t2, 184($fp)
    sw $t1, 0($t2)
    li $t1, 0
    sw $t1, 192($fp)
    li $t1, 2
    sw $t1, 196($fp)
    lw $t2, 196($fp)
    li $t3, 1
    mul $t1, $t2, $t3
    sw $t1, 196($fp)
    lw $t2, 192($fp)
    lw $t3, 196($fp)
    add $t1, $t2, $t3
    sw $t1, 192($fp)
    lw $t2, 192($fp)
    li $t3, 4
    mul $t1, $t2, $t3
    sw $t1, 192($fp)
    la $t2, 4($fp)
    lw $t3, 192($fp)
    add $t1, $t2, $t3
    sw $t1, 192($fp)
    lw $t1, 192($fp)
    sw $t1, 200($fp)
    lw $t1, 200($fp)
    lw $t1, 0($t1)
    sw $t1, 204($fp)
    lw $t1, 168($fp)
    sw $t1, 208($fp)
    lw $t2, 204($fp)
    lw $t3, 208($fp)
    add $t1, $t2, $t3
    sw $t1, 212($fp)
    lw $a0, 212($fp)
    jal write
    li $t1, 0
    sw $t1, 172($fp)
    li $t1, 0
    sw $t1, 216($fp)
    lw $v0, 216($fp)
    jr $ra
