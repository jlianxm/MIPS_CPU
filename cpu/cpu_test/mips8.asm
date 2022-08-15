.text
# To test the data hazard of beq and bne

0x2009000c    00100000000010010000000000001100     addi $t1, $zero, 12
0x200afffe    00100000000010101111111111111110     addi $t2, $zero, -2
0x200b0017    00100000000010110000000000010111     addi $t3, $zero, 23
0x20020000    00100000000000100000000000000000     addi $v0, $zero, 0
0x20030013    00100000000000110000000000010011     addi $v1, $zero, 19
0x20040001    00100000000001000000000000000001     addi $a0, $zero, 1
0x2008ffff    00100000000010001111111111111111     addi $t0, $zero, -1
0x20060001    00100000000001100000000000000001     addi $a2, $zero, 1 # meaningless instruction, just to avoid other hazards
0x20060001    00100000000001100000000000000001     addi $a2, $zero, 1 # meaningless instruction, just to avoid other hazards
0x20060001    00100000000001100000000000000001     addi $a2, $zero, 1 # meaningless instruction, just to avoid other hazards
0x20050000    00100000000001010000000000000000     addi $a1, $zero, 0 # The loop starts here. 
0x20420001    00100000010000100000000000000001     addi $v0, $v0, 1  
0x01044022    00000001000001000100000000100010     sub $t0, $t0, $a0
0x0043282a    00000000010000110010100000101010     slt $a1, $v0, $v1
0x14a4fffb    00010100101001001111111111111011     bne $a1, $a0, -5 # If the data hazard is not handled correctly, the loop will never end
0xac080008    10101100000010000000000000001000     sw $t0, 8($zero)  # if the branch is handled correctly, 
                                                # $t0 shoule be -2, and the value is stored in DATA_MEM[2]
0x20060001    00100000000001100000000000000001     addi $a2, $zero, 1 # meaningless instruction, just to avoid other hazards
0x8c090008    10001100000010010000000000001000     lw $t1, 8($zero)  # $t1 should be -2
0x112a0002    00010001001010100000000000000010     beq $t1, $t2, 2  # since $t1 = $t2, it should branch
0x200b0003    00100000000010110000000000000011     addi $t3, $zero, 3 # If the beq succeeds, this should be skipped. Thus, $t3 should be 23
0x20060001    00100000000001100000000000000001     addi $a2, $zero, 1 # meaningless instruction, just to avoid other hazards
0x20060001    00100000000001100000000000000001     addi $a2, $zero, 1 # meaningless instruction, just to avoid other hazards
0x20060001    00100000000001100000000000000001     addi $a2, $zero, 1 # meaningless instruction, just to avoid other hazards
0xac0b000c    10101100000010110000000000001100     sw $t3, 12($zero) # store $t3 in DATA_MEM[3], which should be 23