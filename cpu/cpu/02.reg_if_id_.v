// This module is the buffer between IF and ID.
module reg_if_id_(clk,reset,PC4,InstructionF,StopF,isLWHazard,PC4D,InstructionD,StopD);

    input wire[31:0] PC4, InstructionF;
    input wire StopF, isLWHazard;
    input wire clk,reset;
    output reg[31:0] PC4D, InstructionD;
	output reg StopD;

    always @(posedge clk) begin

    	if(~reset) begin
    		PC4D <= 32'b00000000;
    		InstructionD <= 32'b00000000;
			StopD <= 1'b0;
		end 
		else begin
			if (~isLWHazard) begin
				PC4D <= PC4;
    	    	InstructionD <= InstructionF;
				StopD <= StopF;
			end
			else begin
				PC4D <= PC4D;
				InstructionD <= InstructionD;
				StopD <= StopD;
			end

    	end
	end

endmodule