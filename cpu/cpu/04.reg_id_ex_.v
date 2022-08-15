// This module is the buffer between ID and EX.
module reg_id_ex_(clk,
                  reset,
                  StopD,
                  RegWriteD,
                  MemtoRegD,
                  MemWriteD,
                  ALUControlD,
                  ALUSrcD,
                  aD,
                  bD,
                  ImmD,
                  rwD,
                  isShiftD,
                  isJalD,
				  isLWHazard,
                  PC4D,
                  RegWriteE,
                  MemtoRegE,
                  MemWriteE,
                  ALUControlE,
                  ALUSrcE,
                  aE,
                  bE,
                  ImmE,
                  rwE_tmp,
                  isShiftE,
                  isJalE,
                  PC4E,
                  StopE);
    
    input wire clk, reset,StopD,RegWriteD, MemtoRegD, MemWriteD, ALUSrcD, isShiftD, isJalD, isLWHazard;
    input wire[31:0] aD, bD, ImmD, PC4D;
    input wire[4:0] rwD, ALUControlD;
    
    output reg RegWriteE, MemtoRegE, MemWriteE, ALUSrcE, isShiftE, isJalE,StopE;
    output reg[31:0] aE, bE, ImmE, PC4E;
    output reg[4:0] rwE_tmp,ALUControlE;
    
    always @(posedge clk) begin
        if ((~reset)) begin
            RegWriteE   <= 0;
            MemtoRegE   <= 0;
            MemWriteE   <= 0;
            ALUSrcE     <= 0;
            isShiftE    <= 0;
            isJalE      <= 0;
            aE          <= 0;
            bE          <= 0;
            ImmE        <= 0;
            PC4E        <= 0;
            rwE_tmp     <= 0;
            ALUControlE <= 0;
            StopE       <= 0;
        end
        else begin
			RegWriteE   <= RegWriteD;
            MemtoRegE   <= MemtoRegD;
            MemWriteE   <= MemWriteD;
            ALUSrcE     <= ALUSrcD;
            isShiftE    <= isShiftD;
            isJalE      <= isJalD;
            aE          <= aD;
            bE          <= bD;
            ImmE        <= ImmD;
            PC4E        <= PC4D;
            rwE_tmp     <= rwD;
            ALUControlE <= ALUControlD;
            StopE       <= StopD;
        end
    end
    
endmodule
