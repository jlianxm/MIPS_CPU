// This module execute the instruction
module ex_(ALUControlE,
           ALUSrcE,
           aE,
           bE,
           ImmE,
           rwE_tmp,
           PC4E,
           isJalE,
           rwE,
           ALUOutE);
    
    input wire[4:0] ALUControlE,rwE_tmp;
    input wire[31:0] aE, bE, ImmE, PC4E;
    input wire ALUSrcE, isJalE;
    
    output wire[4:0] rwE;
    output wire[31:0] ALUOutE;
    
    wire [31:0] ALUBSrc,ALUOut;
    wire [31:0] PC4E_jal;
    assign PC4E_jal = PC4E << 2;
    assign ALUBSrc  = ALUSrcE? ImmE:bE;
    alu_ alu(aE,ALUBSrc,ALUControlE,ALUOut);
    assign ALUOutE = isJalE? PC4E_jal: ALUOut;
    assign rwE     = rwE_tmp | {5{isJalE}}; //write reg no.
    
endmodule
