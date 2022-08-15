// This module is the Memory I/O stage
module mem_(clk,
            reset,
            StopM,
            MemWriteM,
            ALUOutM,
            bM,
            MemOutW,
            MemtoRegM,
            MemOutM);
    
    input wire StopM,MemWriteM,MemtoRegM;
    input wire[31:0] ALUOutM,bM,MemOutW;
    
    input wire clk, reset;
    output wire[31:0] MemOutM;
    
    wire enable;
    wire [31:0] mem_dataout;
    wire [31:0] fetch_address;
    wire [64:0] edit_serial;
    
    assign enable             = MemWriteM | MemtoRegM;
    assign edit_serial[64]    = MemWriteM;
    assign edit_serial[63:32] = ALUOutM;
    assign edit_serial[31:0]  = bM;
    assign fetch_address      = ALUOutM;
    
    //ALUOutM: ALUOutM address bM: data
    wire [31:0] datain;
    assign datain = MemtoRegM?MemOutW:bM;
    
    MainMemory ram(clk,enable,StopM,fetch_address,edit_serial,MemOutM);
    
endmodule
