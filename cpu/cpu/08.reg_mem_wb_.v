// This is the buffer between MEM and RB
module reg_mem_wb(clk,
                  reset,
                  RegWriteM,
                  MemtoRegM,
                  MemOutM,
                  ALUOutM,
                  rwM,
                  RegWriteW,
                  MemtoRegW,
                  MemOutW,
                  ALUOutW,
                  rwW);
    
    input wire RegWriteM,MemtoRegM;
    input wire[31:0] MemOutM,ALUOutM;
    input wire[4:0] rwM;
    input wire clk,reset;
    
    output reg RegWriteW,MemtoRegW;
    output reg[31:0] MemOutW,ALUOutW;
    output reg[4:0] rwW;
    
    always @(posedge clk) begin
        if (~reset) begin
            RegWriteW <= 0;
            MemtoRegW <= 0;
            MemOutW   <= 32'b00000000;
            ALUOutW   <= 32'b00000000;
            rwW       <= 5'b00000;
        end
        else begin
            RegWriteW <= RegWriteM;
            MemtoRegW <= MemtoRegM;
            MemOutW   <= MemOutM;
            ALUOutW   <= ALUOutM;
            rwW       <= rwM;
        end
    end
    
endmodule
