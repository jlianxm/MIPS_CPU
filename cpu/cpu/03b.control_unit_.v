// This module generate the corresponding signals of the input instruction
module control_unit_(
    // input
    // R-TYPE
    // Arithmetic instruction
    input ADD,
    input ADDU,
    input SUB,
    input SUBU,

    // Logical instruction
    input AND,
    input NOR,
    input OR,
    input XOR,

    // Shifting instruction
    input SLL,
    input SLLV,
    input SRL,
    input SRLV,
    input SRA,
    input SRAV,

    // Branch/Jump instruction
    input SLT,

    // I-TYPE
    // Arithmetic instruction
    input ADDI,
    input ADDIU,

    // Logical instruction
    input ANDI,
    input ORI,
    input XORI,

    // Data transfer instruction
    input LW,
    input SW,

    // Branch/Jump instruction
    input BEQ,
    input BNE,

    // J-TYPE
    // Jump instruction
    input J,
    input JR,
    input JAL,

    // BRANCH OR NOT
    input isBranchHazard,

    // control signals
    output  RegWrite,
    output  MemtoReg,
    output  MemWrite,
    output [4:0] ALUControl,
    output  ALUSrc,
    output  RegDstRt,
    output  SignExt,
    output  isShift,
    output  isJal,
    output [1:0] PCSource
);
    assign    PCSource[1] = J||JR||JAL;
    assign    PCSource[0] = (BEQ&&isBranchHazard)||(BNE&&~isBranchHazard)||J||JAL||JR;

    assign    RegWrite = ADD||ADDU||SUB||SUBU||AND||NOR||OR||XOR||SLL||SLLV||SRL||SRLV||SRA||SRAV||SLT||ADDI||ADDIU||ANDI||ORI||XORI||LW||JAL;

    assign    ALUControl[0] = ADDU||SUBU||NOR||XOR||SLLV||SRLV||SRAV||ADDI||ANDI||XORI||SW||BNE;
    assign    ALUControl[1] = SUB||SUBU||OR||XOR||SRL||SRLV||SLT||ADDI||ORI||XORI||BEQ||BNE;    
    assign    ALUControl[2] = AND||NOR||OR||XOR||SRA||SRAV||SLT||ADDI||LW||SW||BEQ||BNE;
    assign    ALUControl[3] = SLL||SLLV||SRL||SRLV||SRA||SRAV||SLT||ADDI;
    assign    ALUControl[4] = ADDIU||ANDI||ORI||XORI||LW||SW||BEQ||BNE;
    
    assign    isShift = SLL||SRL||SRA;
    // assign    isBranch = BEQ||BNE;

    // 1: data memory 0:alu
    assign    ALUSrc = ADDI||ADDIU||ANDI||ORI||XORI||LW||SW;
    assign    SignExt  = ADDI||ADDIU||LW||SW||BNE||BEQ;
    
    assign    MemWrite = SW;
    assign    MemtoReg = LW;
        
    // 1:rt   0:rd
    assign    RegDstRt = ~(ADD||ADDU||SUB||SUBU||AND||NOR||OR||XOR||SLL||SLLV||SRL||SRLV||SRA||SRAV||SLT);

    assign    isJal = JAL;


endmodule
