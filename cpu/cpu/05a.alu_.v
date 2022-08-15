// This module is the ALU
module alu_(A, B, ALUControl, result);
    
    // A: [25-21] RS; B: [20-16] or imm
    input  [31:0] A, B;
    input [4:0] ALUControl;
    output [31:0] result;
    
    integer i; //Loop counter
    reg signed [31:0] temp,reg_result,regA, regB;
    assign result = reg_result;
    
    always @ (*) begin

        regA = A;   
        regB = B;

        case (ALUControl)
            5'b00000:   reg_result = regA + regB;                   // ADD
            5'b00001:   reg_result = regA + regB;                   // ADDU
            5'b00010:   reg_result = regA - regB;                   // SUB
            5'b00011:   reg_result = regA - regB;                   // SUBU
            5'b00100:   reg_result = regA & regB;                   // AND
            5'b00101:   reg_result = ~(regA | regB);                // NOR
            5'b00110:   reg_result = regA | regB;                   // OR
            5'b00111:   reg_result = regA ^ regB;                   // XOR,
            5'b01000:   reg_result = (regB << regA);                     // SLL
            5'b01001:   reg_result = (regB << regA[4:0]);                     // SLLV
            5'b01010:   reg_result = (regB >> regA);                     // SRL  PROB!!!
            5'b01011:   reg_result = (regB >> regA[4:0]);                     // SRLV
            5'b01100:                             // SRA
                begin
                    temp = regB;
                    for(i = 0; i < regA; i = i + 1) begin
                        temp = {temp[31],temp[31:1]}; //add the lsb for msb
                    end
                    reg_result = temp;
                end
            5'b01101:                                    // SRAV
                begin
                    temp = regB;
                    for(i = 0; i < regA[4:0]; i = i + 1) begin
                        temp = {temp[31],temp[31:1]}; //add the lsb for msb
                    end
                reg_result = temp;
                end
            5'b01110:                                    // SLT
                begin
                    if(regA < regB) begin
                        reg_result=1;
                    end 
                    else begin
                        reg_result=0;
                    end
                end  
            5'b01111:    reg_result = regA + regB;                                // ADDI
            5'b10000:    reg_result = regA + regB;                                // ADDIU
            5'b10001:    reg_result = regA & {16'b0,regB[15:0]};                                // ANDI
            5'b10010:    reg_result = regA | {16'b0,regB[15:0]};                                // ORI
            5'b10011:    reg_result = regA ^ {16'b0,regB[15:0]};                                // XORI
            5'b10100:    
            begin
                reg_result = regA + regB;                                // LW
                reg_result = reg_result >> 2;
            end
            
            5'b10101:
                begin
                    reg_result = regA + regB;                                // SW
                    reg_result = reg_result >> 2;
                end    
        endcase
    end
	
endmodule