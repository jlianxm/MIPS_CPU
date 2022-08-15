// This module is the buffer between EX and MEM
module reg_ex_mem_(clk,
                   reset,
                   StopE,
                   RegWriteE,
                   MemtoRegE,
                   MemWriteE,
                   ALUOutE,
                   bE,
                   rwE,
                   RegWriteM,
                   MemtoRegM,
                   MemWriteM,
                   ALUOutM,
                   bM,
                   rwM,
                   StopM);
    
    input wire clk, reset,RegWriteE, MemtoRegE, MemWriteE,StopE;
    input wire[31:0] ALUOutE,bE;
    input wire[4:0] rwE;
    
    output reg RegWriteM, MemtoRegM, MemWriteM,StopM;
    output reg[31:0] ALUOutM,bM;
    output reg[4:0] rwM;
    
    always @(posedge clk) begin
        if (~reset) begin
            RegWriteM <= 0;
            MemtoRegM <= 0;
            MemWriteM <= 0;
            ALUOutM   <= 0;
            bM        <= 0;
            rwM       <= 0;
            StopM     <= 0;
        end
        else begin
            RegWriteM <= RegWriteE;
            MemtoRegM <= MemtoRegE;
            MemWriteM <= MemWriteE;
            ALUOutM   <= ALUOutE;
            bM        <= bE;
            rwM       <= rwE;
            StopM     <= StopE;
        end
    end
    
endmodule
