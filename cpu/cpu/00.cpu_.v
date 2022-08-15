`include "01.if_.v"
`include "01a.InstructionRAM.v"
`include "02.reg_if_id_.v"
`include "03.id_.v"
`include "03a.instruction_docoder_.v"
`include "03b.control_unit_.v"
`include "03c.registers_.v"
`include "04.reg_id_ex_.v"
`include "05.ex_.v"
`include "05a.alu_.v"
`include "06.reg_ex_mem_.v"
`include "07.mem_.v"
`include "07a.MainMemory.v"
`include "08.reg_mem_wb_.v"
`include "X1.mux4_32_.v"
`include "X2.bubble_inserter_.v"
`include "X3.forwarding_unit_.v"

module pipelined_computer(reset,clk);

	input reset,clk;
	//IF
	wire StopF;
	wire [31:0] PC_B,PC_J,PC4,InstructionF,InstructionD;
	//ID
	wire [31:0] PC4D,aD,bD,ImmD;
	//EX
	wire [31:0] PC4E,aE,bE,ImmE,ALUOutE;
	//MEM
	wire [31:0] bM,MemOutM,ALUOutM;
	//WB
	wire [31:0] MemOutW,WBSrc,ALUOutW;
	// the number of the register to write
	wire [4:0] rwD,rwE_tmp,rwE,rwM,rwW;
	//ALU
	wire [4:0] ALUControlD,ALUControlE;
	//PCsource
	wire [1:0] PCSource;

	// signal of the corresponding instruction
	//IF/ID
	wire RegWriteD,MemtoRegD,MemWriteD,ALUSrcD,isShiftD,isJalD,StopD;
	//ID/EX
	wire RegWriteE,MemtoRegE,MemWriteE,ALUSrcE,isShiftE,isJalE,StopE;
	//EX/MEM
	wire RegWriteM,MemtoRegM,MemWriteM,StopM;
	//MEM/WB
	wire RegWriteW,MemtoRegW;

	// instruction fetch stage
	if_ instruction_fetch (clk,reset,PCSource,PC_B,aD,PC_J,PC4,isLWHazard,InstructionF,StopF);
	
	// buffer between instruction fetch and instruction decode
	reg_if_id_ reg_if_id (clk,reset,PC4,InstructionF,StopF,isLWHazard,PC4D,InstructionD,StopD);

	// instruction decode stage
	id_ instruction_decode (clk,reset,RegWriteM,rwM,rwE,RegWriteE,MemtoRegE,MemtoRegM,PC4D,InstructionD,
		rwW,WBSrc,ALUOutE,ALUOutM,MemOutM,RegWriteW,PC_B,
		PC_J,PCSource,RegWriteD,MemtoRegD,MemWriteD,ALUControlD,ALUSrcD,
		aD,bD,ImmD,rwD,isShiftD,isJalD,isLWHazard);

	// buffer between instruction decode and execute
	reg_id_ex_ reg_id_ex(clk,reset,StopD,RegWriteD,MemtoRegD,MemWriteD,ALUControlD,ALUSrcD,aD,bD,ImmD,
		rwD,isShiftD,isJalD,isLWHazard,PC4D,RegWriteE,MemtoRegE,MemWriteE,ALUControlE,ALUSrcE,
		aE,bE,ImmE,rwE_tmp,isShiftE,isJalE,PC4E,StopE);
	
	// execute stage
	ex_ execute (ALUControlE,ALUSrcE,aE,bE,ImmE,rwE_tmp,PC4E,isJalE,rwE,ALUOutE);
	
	// buffer between execute and memory
	reg_ex_mem_ reg_ex_mem (clk,reset,StopE,RegWriteE,MemtoRegE,MemWriteE,ALUOutE,bE,rwE,RegWriteM,
		MemtoRegM,MemWriteM,ALUOutM,bM,rwM,StopM);
	
	// memory stage
	mem_ memory (clk,reset,StopM,MemWriteM,ALUOutM,bM,MemOutW,MemtoRegM,MemOutM);
	
	// buffter between memory and write back
	reg_mem_wb reg_mem_wb(clk,reset,RegWriteM,MemtoRegM,MemOutM,ALUOutM,rwM,RegWriteW,
		MemtoRegW,MemOutW,ALUOutW,rwW);
	
	// choose to write back the result from ALU or memory
	assign WBSrc = MemtoRegW? MemOutW:ALUOutW;

endmodule