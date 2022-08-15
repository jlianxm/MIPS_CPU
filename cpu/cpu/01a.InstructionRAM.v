// This module is the RAM of instruction
module InstructionRAM
    ( // Inputs
    input stop,
    input [31:0] FETCH_ADDRESS, // pc

    // Outputs
    output reg [31:0] DATA // fetched instruction
    );

  reg [31:0] RAM [0:512-1];

  initial begin
    $readmemb("../cpu_test/machine_code.txt",RAM);
  end

  always @(FETCH_ADDRESS) begin : InstructionRAM_blockRamFile
    if (~stop) begin
      DATA = RAM[(FETCH_ADDRESS)];
    end
  end


endmodule

