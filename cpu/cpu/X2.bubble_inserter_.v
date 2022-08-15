// This module inserts a bubble into ID/EX when LW hazards occurs.
module bubble_inserter_(clk,
                        reset,
                        StopD_tmp,
                        RegWriteD_tmp,
                        MemtoRegD_tmp,
                        MemWriteD_tmp,
                        ALUControlD_tmp,
                        ALUSrcD_tmp,
                        rwD_tmp,
                        isShiftD_tmp,
                        isJalD_tmp,
                        isLWHazard,
                        RegWriteD,
                        MemtoRegD,
                        MemWriteD,
                        ALUControlD,
                        ALUSrcD,
                        rwD,
                        isShiftD,
                        isJalD);

    input wire clk, reset,StopD_tmp,RegWriteD_tmp, MemtoRegD_tmp, MemWriteD_tmp, ALUSrcD_tmp, isShiftD_tmp, isJalD_tmp, isLWHazard;
    input wire[31:0] PC4D_tmp;
    input wire[4:0] rwD_tmp, ALUControlD_tmp;
    
    // if is a bubble, *E_tmp = 0; otherwise, *E_tmp = *D;
    output reg RegWriteD, MemtoRegD, MemWriteD, ALUSrcD, isShiftD, isJalD,StopD;
    output reg[4:0] rwD, ALUControlD;
    
    
    always @(*) begin
        if (isLWHazard) begin
            RegWriteD   <= 0;
            MemtoRegD   <= 0;
            MemWriteD   <= 0;
            ALUSrcD     <= 0;
            isShiftD    <= 0;
            isJalD      <= 0;
            rwD         <= 0;
            ALUControlD <= 0;
            StopD       <= 0;
        end
        else begin
            RegWriteD   <= RegWriteD_tmp;
            MemtoRegD   <= MemtoRegD_tmp;
            MemWriteD   <= MemWriteD_tmp;
            ALUSrcD     <= ALUSrcD_tmp;
            isShiftD    <= isShiftD_tmp;
            isJalD      <= isJalD_tmp;
            rwD         <= rwD_tmp;
            ALUControlD <= ALUControlD_tmp;
            StopD       <= StopD_tmp;
        end
    end
    
    
    
endmodule
