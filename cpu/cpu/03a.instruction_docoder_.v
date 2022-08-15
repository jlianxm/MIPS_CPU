// This module recognize the type of instruction
module instruction_decoder_(
    input [31:0]inst,
    input [5:0]op,
    input [5:0]func,
    // R-TYPE
    // Arithmetic instruction
    output ADD,
    output ADDU,
    output SUB,
    output SUBU,

    // Logical instruction
    output AND,
    output NOR,
    output OR,
    output XOR,

    // Shifting instruction
    output SLL,
    output SLLV,
    output SRL,
    output SRLV,
    output SRA,
    output SRAV,

    // Branch/Jump instruction
    output SLT,

    // I-TYPE
    // Arithmetic instruction
    output ADDI,
    output ADDIU,

    // Logical instruction
    output ANDI,
    output ORI,
    output XORI,

    // Data transfer instruction
    output LW,
    output SW,

    // Branch/Jump instruction
    output BEQ,
    output BNE,

    // J-TYPE
    // Jump instruction
    output J,
    output JR,
    output JAL
);

    reg [26:0]instruction_type;

    // R-TYPE
    // Arithmetic instruction
    assign ADD=instruction_type[0];
    assign ADDU=instruction_type[1];
    assign SUB=instruction_type[2];
    assign SUBU=instruction_type[3];

    // Logical instruction
    assign AND=instruction_type[4];
    assign NOR=instruction_type[5];
    assign OR=instruction_type[6];
    assign XOR=instruction_type[7];

    // Shifting instruction
    assign SLL=instruction_type[8];
    assign SLLV=instruction_type[9];
    assign SRL=instruction_type[10];
    assign SRLV=instruction_type[11];
    assign SRA=instruction_type[12];
    assign SRAV=instruction_type[13];

    // Branch/Jump instruction
    assign SLT=instruction_type[14];

    // I-TYPE
    // Arithmetic instruction
    assign ADDI=instruction_type[15];
    assign ADDIU=instruction_type[16];

    // Logical instruction
    assign ANDI=instruction_type[17];
    assign ORI=instruction_type[18];
    assign XORI=instruction_type[19];

    // Data transfer instruction
    assign LW=instruction_type[20];
    assign SW=instruction_type[21];

    // Branch/Jump instruction
    assign BEQ=instruction_type[22];
    assign BNE=instruction_type[23];

    // J-TYPE
    // Jump instruction
    assign J=instruction_type[24];
    assign JR=instruction_type[25];
    assign JAL=instruction_type[26];
    
    
    always @(*) begin
        if (op ==6'h0) begin
            case(func)
                
                6'h20:instruction_type={26'b0,1'b1};        //ADD
                6'h21:instruction_type={25'b0,1'b1,1'b0};   //ADDU
                6'h22:instruction_type={24'b0,1'b1,2'b0};   //SUB
                6'h23:instruction_type={23'b0,1'b1,3'b0};   //SUBU

                6'h24:instruction_type={22'b0,1'b1,4'b0};   //AND
                6'h27:instruction_type={21'b0,1'b1,5'b0};   //NOR
                6'h25:instruction_type={20'b0,1'b1,6'b0};   //OR
                6'h26:instruction_type={19'b0,1'b1,7'b0};   //XOR

                6'h00:
                begin
                    if (inst != 32'h00000000) begin
                        instruction_type={18'b0,1'b1,8'b0};   //SLL
                    end 
                    else begin
                        instruction_type={27'h0};
                    end
                end

                6'h04:instruction_type={17'b0,1'b1,9'b0};   //SLLV
                6'h02:instruction_type={16'b0,1'b1,10'b0};   //SRL
                6'h06:instruction_type={15'b0,1'b1,11'b0};   //SRLV
                6'h03:instruction_type={14'b0,1'b1,12'b0};   //SRA
                6'h07:instruction_type={13'b0,1'b1,13'b0};   //SRAV
                
                6'h2A:instruction_type={12'b0,1'b1,14'b0};    //SLT
                6'h08:instruction_type={1'b0,1'b1,25'b0};       //JR

                default:instruction_type={27'h0};
            endcase
        end

        else if(op==6'h8)   //ADDI
            instruction_type={11'b0,1'b1,15'b0};
        else if(op==6'h9)   //ADDIU       
            instruction_type={10'b0,1'b1,16'b0};

        else if(op==6'hC)   //ANDI
            instruction_type={9'b0,1'b1,17'b0};
        else if(op==6'hD)   //ORI
            instruction_type={8'b0,1'b1,18'b0};
        else if(op==6'hE)   //XORI
            instruction_type={7'b0,1'b1,19'b0};
        
        else if(op==6'h23)   //LW
            instruction_type={6'b0,1'b1,20'b0};
        else if(op==6'h2B)   //SW
            instruction_type={5'b0,1'b1,21'b0};
        
        else if(op==6'h4)   //BEQ
            instruction_type={4'b0,1'b1,22'b0};
        else if(op==6'h5)   //BNE
            instruction_type={3'b0,1'b1,23'b0};
        
        else if(op==6'h2)   //J
            instruction_type={2'b0,1'b1,24'b0};
        else if(op==6'h3)   //JAL
            instruction_type={1'b1,26'b0};

        else
          instruction_type={27'h00}; // STOP
    end
endmodule
