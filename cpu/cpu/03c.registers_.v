// This module is the register file in ID stage.
module registers_ (rna,
                   rnb,
                   WBSrc,
                   rwW,
                   RegWriteW,
                   clk,
                   reset,
                   rf_outa,
                   rf_outb);
    
    input [4:0] rna,rnb,rwW;
    input [31:0] WBSrc;
    input RegWriteW,clk,reset;
    
    output [31:0] rf_outa,rf_outb;
    
    reg [31:0] register [1:31]; // r1 - r31
    reg [31:0] reg_qa;
    reg [31:0] reg_qb;
    
    assign rf_outa = reg_qa;
    assign rf_outb = reg_qb;
    
    integer reg_i;
    
    // read at negedge
    always @(negedge clk or negedge reset) begin
        
        if (reset == 0) begin // reset
            for (reg_i = 1; reg_i<32; reg_i = reg_i+1)
                register[reg_i] <= 0;
        end
        else begin
            reg_qa <= (rna==0)? 32'b00000000 : register[rna]; // read
            reg_qb <= (rnb==0)? 32'b00000000 : register[rnb];
        end
        
    end
    
    // write no edge
    always @(*) begin
        if ((rwW != 0) && (RegWriteW == 1)) begin
            register[rwW] = WBSrc;
        end
    end
    
    
endmodule
