.text
# To test the hazard of lw stall

20020072    00100000000000100000000001110010         addi $v0, $zero, 114
20030202    00100000000000110000001000000010         addi $v1, $zero, 514
201a0001    00100000000110100000000000000001         addi $k0, $zero, 1 # meaningless instruction, just to avoid other hazards
201a0001    00100000000110100000000000000001         addi $k0, $zero, 1 # meaningless instruction, just to avoid other hazards
201a0001    00100000000110100000000000000001         addi $k0, $zero, 1 # meaningless instruction, just to avoid other hazards
ac020004    10101100000000100000000000000100         sw $v0, 4($zero) # 114 in DATA_MEM[1]
201a0001    00100000000110100000000000000001         addi $k0, $zero, 1 # meaningless instruction, just to avoid other hazards
8c040004    10001100000001000000000000000100         lw $a0, 4($zero)  # $a0 should be 114
00642822    00000000011001000010100000100010         sub $a1, $v1, $a0  # The hazard of lw stall happens
                                            # $a1 shoule be 400
201a0001    00100000000110100000000000000001         addi $k0, $zero, 1 # meaningless instruction, just to avoid other hazards
201a0001    00100000000110100000000000000001         addi $k0, $zero, 1 # meaningless instruction, just to avoid other hazards
201a0001    00100000000110100000000000000001         addi $k0, $zero, 1 # meaningless instruction, just to avoid other hazards                   
ac050008    10101100000001010000000000001000         sw $a1, 8($zero)  # 400 in DATA_MEM[2]