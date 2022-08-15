.text
# To test the data hazard about jr

0x20020048    addi $v0, $zero, 72
0x201a0068    addi $k0, $zero, 104
0x2010ffff    addi $s0, $zero, -1
0x2011fffe    addi $s1, $zero, -2
0x2012fffc    addi $s2, $zero, -4
0x2013fff8    addi $s3, $zero, -8
0x2014fff0    addi $s4, $zero, -16
0x2015ffe0    addi $s5, $zero, -32
0x20040001    addi $a0, $zero, 1
0x20050001    addi $a1, $zero, 1
0x20060001    addi $a2, $zero, 1
0x20080001    addi $t0, $zero, 1
0x20090001    addi $t1, $zero, 1
0x200a0001    addi $t2, $zero, 1
0x20030074    addi $v1, $zero, 116
0xac030000    sw $v1, 0($zero) # 116 is stored in MEM[0]
0x20020054    addi $v0, $zero, 84
0x00400008    jr $v0 # The first jr
0x20100003    addi $s0, $zero, 3 # if jr succeeds, this should be skipped
0x20110003    addi $s1, $zero, 3 # if jr succeeds, this should be skipped
0x20120003    addi $s2, $zero, 3 # if jr succeeds, this should be skipped
                # first jr jumps to here
0x20040002    addi $a0, $zero, 2 # $a0 shoule be 2  <-- 21
0x20050004    addi $a1, $zero, 4 # $a1 should be 4
0x20060008    addi $a2, $zero, 8 # $a2 should be 8
0x8c1a0000    lw $k0, 0($zero) # $k0 is 116
0x03400008    jr $k0  # The second jr
0x20130003    addi $s3, $zero, 3 # if jr succeeds, this should be skipped
0x20140003    addi $s4, $zero, 3 # if jr succeeds, this should be skipped
0x20150003    addi $s5, $zero, 3 # if jr succeeds, this should be skipped
                # second jr jumps to here
0x20080010    addi $t0, $zero, 16 # $t0 shoule be 16  <-- 29
0x20090020    addi $t1, $zero, 32 # $t1 should be 32
0x200a0040    addi $t2, $zero, 64 # $t2 should be 64
0xac040000    sw $a0, 0($zero)  # 2 in DATA_MEM[0]
0xac050004    sw $a1, 4($zero)  # 4 in DATA_MEM[1]
0xac060008    sw $a2, 8($zero)  # 8 in DATA_MEM[2]
0xac08000c    sw $t0, 12($zero) # 16 in DATA_MEM[3]
0xac090010    sw $t1, 16($zero) # 32 in DATA_MEM[4]
0xac0a0014    sw $t2, 20($zero) # 64 in DATA_MEM[5]
0xac100018    sw $s0, 24($zero) # -1 in DATA_MEM[6]
0xac11001c    sw $s1, 28($zero) # -2 in DATA_MEM[7]
0xac120020    sw $s2, 32($zero) # -4 in DATA_MEM[8]
0xac130024    sw $s3, 36($zero) # -8 in DATA_MEM[9]
0xac140028    sw $s4, 40($zero) # -16 in DATA_MEM[10]
0xac15002c    sw $s5, 44($zero) # -32 in DATA_MEM[11]
