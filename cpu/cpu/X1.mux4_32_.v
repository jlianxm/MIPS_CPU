module mux4_32_ (a0,
                 a1,
                 a2,
                 a3,
                 s,
                 out);
    
    input [31:0]  a0,a1,a2,a3;
    input [1:0]  s;
    
    output [31:0] out;
    reg [31:0] out;
    
    
    always @(*) begin
        
        case (s)
            
            2'b00: out = a0;
            2'b01: out = a1;
            2'b10: out = a2;
            2'b11: out = a3;
            
        endcase
        
    end
    
    // assign out = y;
    
endmodule
