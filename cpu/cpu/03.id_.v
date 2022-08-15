// This module is the instruction decode stage.
module id_(clk,reset,RegWriteM,rwM,rwE,RegWriteE,MemtoRegE,MemtoRegM,PC4D,InstructionD,
	rwW,WBSrc,ALUOutE,ALUOutM,MemOutM,RegWriteW,PC_B,
	PC_J,PCSourceD,RegWriteD,MemtoRegD,MemWriteD,ALUControlD,ALUSrcD,
	aD,bD,ImmD,rwD,isShiftD,isJalD,isLWHazard);
    
    input wire clk,reset;
    input wire RegWriteM,RegWriteE,MemtoRegE,MemtoRegM,RegWriteW;
    input wire [4:0] rwM,rwE,rwW;
    input wire [31:0] PC4D,InstructionD,WBSrc,ALUOutE,ALUOutM,MemOutM;
    
    output wire RegWriteD,MemtoRegD,MemWriteD,ALUSrcD,isShiftD,isJalD,isLWHazard;
    output wire [4:0] ALUControlD;

    output wire [31:0] aD,bD,ImmD,PC_B,PC_J;
    output wire [4:0] rwD;
    output wire [1:0] PCSourceD;
    
    wire RegDstRtD,SignExtD; 
    
    //aD==bD branch hazard
    wire isBranchHazard = ~|(aD^bD); 
    
    wire[5:0] op = InstructionD[31:26];
    wire[5:0] func = InstructionD[5:0];
    
    // R-type decode
    wire[4:0] rs = InstructionD[25:21];
    wire[4:0] rt = InstructionD[20:16];
    wire[4:0] rd = InstructionD[15:11];

    // The bit-shift amount
    wire [31:0] sa = { 27'b0, InstructionD[10:6] };
    
    // rf_out: register file out
    wire [31:0] rf_outa,rf_outb,PC_JR,PC_J_JAL;

    // *_tmp: tmp signal, determined before checking lw hazard or not
    wire StopD_tmp, RegWriteD_tmp,MemtoRegD_tmp,MemWriteD_tmp,ALUSrcD_tmp,isShiftD_tmp,isJalD_tmp;
    wire[4:0] ALUControlD_tmp, rwD_tmp;
    
    instruction_decoder_ ins_de(InstructionD,op,func,ADD,ADDU,SUB,SUBU,AND,NOR,OR,XOR,SLL,SLLV,SRL,SRLV,SRA,SRAV,
                        SLT,ADDI,ADDIU,ANDI,ORI,XORI,LW,SW,BEQ,BNE,J,JR,JAL);

    control_unit_ cu(ADD,ADDU,SUB,SUBU,AND,NOR,OR,XOR,SLL,SLLV,SRL,SRLV,SRA,SRAV,SLT,ADDI,ADDIU,ANDI,
                        ORI,XORI,LW,SW,BEQ,BNE,J,JR,JAL,isBranchHazard,
                        RegWriteD_tmp,MemtoRegD_tmp,MemWriteD_tmp,ALUControlD_tmp,ALUSrcD_tmp,
                        RegDstRtD,SignExtD,isShiftD_tmp,isJalD_tmp,PCSourceD);

    registers_ rf(rs,rt,WBSrc,rwW,RegWriteW,clk,reset,rf_outa,rf_outb);
    
    // roughly implement: assign rwD = isJALD?{5{1}}:(RegDstRtD?rt:rd);
    assign rwD_tmp = RegDstRtD?rt:rd;
    
    // jump hazard
    // roughly implement:
    // if yes, data hazard happends, the register to read have just been written
    assign PC_JR = MemtoRegM? (MemOutM>>2):((JR && (rwE == rs))? (ALUOutE>>2):(rf_outa>>2));
    
    wire J_sign_bit = PC4D[31];
    assign PC_J_JAL = {{2{J_sign_bit}},PC4D[31:28],InstructionD[25:0]};
    assign PC_J = JR? PC_JR : PC_J_JAL;

    // sign extension 
    wire sign_bit = SignExtD&InstructionD[15];
    wire[15:0] imm = {16{sign_bit}};
    assign ImmD = {imm, InstructionD[15:0]};

    // branch pc
    wire[31:0] offset = {{18{sign_bit}}, InstructionD[13:0]}; // 18 bits sign extension according to the manual
    assign PC_B = PC4D + offset;
    
    //data hazards: forwarding
    wire[1:0] ForwardA, ForwardB;
    forwarding_unit_ fu(RegWriteE,MemtoRegE,RegWriteM,MemtoRegM,rwE,rwM,rs,rt,RegDstRtD,MemWriteD_tmp,ForwardA,ForwardB);

    wire[31:0] da_tmp;
    assign da_tmp = isShiftD?sa:rf_outa;

    mux4_32_ forwarding_da(da_tmp,ALUOutE,ALUOutM,MemOutM,ForwardA,aD);
    mux4_32_ forwarding_db(rf_outb,ALUOutE,ALUOutM,MemOutM,ForwardB,bD);


    // hazard detection unit
    assign isLWHazard = (MemtoRegE&((rwE==rt)|(rwE==rs))&(rwM!=0));

    // mux of signals -> empty the signals to send out a bubble
    // insert a bubble when detect LW hazards
    bubble_inserter_ bi(
        clk,reset,StopD_tmp,RegWriteD_tmp,MemtoRegD_tmp,MemWriteD_tmp,ALUControlD_tmp,
        ALUSrcD_tmp,rwD_tmp,isShiftD_tmp,isJalD_tmp,isLWHazard,RegWriteD,MemtoRegD,
        MemWriteD,ALUControlD,ALUSrcD,rwD,isShiftD,isJalD
    );



endmodule