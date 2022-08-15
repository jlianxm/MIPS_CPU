// This module is the instruction fetch stage
module if_(clk,
           reset,
           PCSource,
           PC_B,
           aD,
           PC_J,
           PC4,
           isLWHazard,
           InstructionF,
           StopF);
    
    input wire[1:0] PCSource;
    input wire[31:0] PC_B,aD,PC_J;
    input wire clk,reset,isLWHazard;

    output StopF;
    output wire[31:0] InstructionF;
    inout [31:0] PC4;
    
    reg [31:0] reg_pc,reg_pc4;
    wire [31:0] wire_pc,next_pc;
    wire[31:0] fetched_ins, nop;
    
    reg stop = 0;
    
    always @(posedge clk) begin
        if (~reset) begin
            reg_pc <= 32'h00000000;
        end
        else begin
            if (~isLWHazard) begin
                reg_pc <= next_pc;
            end
            else begin 
                // if is LW hazard, don't change PC
                reg_pc <= reg_pc;
            end

        end
        
        if (fetched_ins == 32'hffffffff) begin
            stop = 1;
        end
        else begin
            stop = 0;
        end
        
    end
    
    assign StopF = stop;
    
    assign wire_pc = reg_pc;
    
    InstructionRAM ram(StopF,wire_pc,fetched_ins);
    
    assign nop = 32'b00000000;
    
    assign InstructionF = PCSource[0]? nop:fetched_ins;
    
    assign PC4 = wire_pc+32'h00000001;
    
    mux4_32_ new_pc(PC4,PC_B,aD,PC_J,PCSource,next_pc);
    
endmodule
