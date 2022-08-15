`timescale 100fs/100fs
`include "00.cpu_.v"

module test_cpu;
	
	reg reset, clk;
    wire stop;
	
	pipelined_computer cpu(
        .reset    (reset),
        .clk  (clk)
    );

   initial begin

    $dumpfile("test_cpu.vcd");
    $dumpvars(0,cpu);

    clk = 1;
    reset = 0;
    #3 reset = 1;

    // $display("testing...");

    #1000 $stop;

    end

    always #1 clk = ~clk;  
    integer i = 0;



endmodule

// iverilog -o test_cpu.vvp test_cpu.v;vvp test_cpu.vvp;gtkwave.exe test_cpu.vcd
