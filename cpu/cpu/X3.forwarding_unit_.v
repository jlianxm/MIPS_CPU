// This module solves R-type data hazards and lw hazard by forwarding.
// conditions:
// 1 EX write data into registers
// 2 The written data is not from memory
// 3 Rd == Rs/Rt
// 4 Rd != 0 (the destination register is not $0)
// For R-type data hazards
// Output parameters:
// ForwardA:
// ForwardA = 00: ID/EX; The first ALU operand comes from the register file -> no hazard
// ForwardA = 01: EX/MEM; The first ALU operand is forwarded from the prior result -> Rd == Rs
// ForwardA = 10: MEM/WB; The first ALU operand is forwarded from data memory or an earlier ALU result
// ForwardA = 11: LW hazards
// ForwardB:
// ForwardB = 00: ID/EX; The second ALU operand comes from the register file -> no hazard
// ForwardB = 01: EX/MEM; The second ALU operand is forwarded from the prior result -> Rd == Rt
// ForwardB = 10: MEM/WB; The second ALU operand is forwarded from data memory or an earlier ALU result
// ForwardB = 11: LW hazards
// For lw hazard
// (MemtoRegE&(rwE==rt)&(rwM!=0))
module forwarding_unit_(RegWriteE,
                        MemtoRegE,
                        RegWriteM,
                        MemtoRegM,
                        rwE,
                        rwM,
                        rs,
                        rt,
                        RegDstRtD,
                        MemWriteD,
                        ForwardA,
                        ForwardB);

    input wire RegWriteE, MemtoRegE, RegWriteM, MemtoRegM, RegDstRtD,MemWriteD;
    input wire [4:0] rs, rt, rwE, rwM;
    output wire [1:0] ForwardA, ForwardB;
    
    assign ForwardA[0] = (RegWriteE&(~MemtoRegE)&(rwE==rs)&(rwE!=0))|(MemtoRegM&(rwM==rs)&(rwM!=0));
    assign ForwardA[1] = (RegWriteM&(~MemtoRegM)&(rwM==rs)&(rwE!=rs)&(rwM!=0))|(MemtoRegM&(rwM==rs)&(rwM!=0));
    
    assign ForwardB[0] = (RegWriteE&(~MemtoRegE)&(rwE==rt)&(rwE!=0)&((~RegDstRtD)|MemWriteD))|(MemtoRegM&(rwM==rt)&(rwM!=0)); 
    assign ForwardB[1] = (RegWriteM&(~MemtoRegM)&(rwM==rt)&(rwE!=rt)&(rwM!=0)&((~RegDstRtD)|MemWriteD))|(MemtoRegM&(rwM==rt)&(rwM!=0));

endmodule
