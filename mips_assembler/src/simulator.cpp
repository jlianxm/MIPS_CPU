#include <string>
#include "mips_funct.h"
#include "simulator.h"

Simulator::Simulator(int num_of_byte)
{
    // initialize the registers
    for (int i = 0; i < 32; ++i) 
    {
        rgt[i] = 0;
    }

    rgt[29] = 0xA00000;
    hi = 0;
    lo = 0;
    rgt_pc = 0x400000;
    dm_end = 0x500000 + num_of_byte;

}

void Simulator::simulate(uint32_t* pc_ptr, uint32_t* pc_start, uint32_t* memory, std::ifstream& syscall_inputs, std::ofstream& output_file)
{

    // pc_start: start; pc_ptr: end; pt current position;
    for (uint32_t *pt = pc_start; pt != pc_ptr; pt++)
    {
        std::string bin_code = mips_simulator::change_order_32(mips_simulator::dec2bin_u(std::to_string(*pt), 32));
        std::string s_rs = bin_code.substr(6, 5);
        std::string s_rt = bin_code.substr(11, 5);
        std::string s_rd = bin_code.substr(16, 5);
        std::string s_shamt = bin_code.substr(21, 5);
        std::string s_imm = bin_code.substr(16, 16);
        std::string s_add = bin_code.substr(6, 26);
        uint32_t i_rs = mips_simulator::bin2dec_u(s_rs);
        uint32_t i_rt = mips_simulator::bin2dec_u(s_rt);
        uint32_t i_rd = mips_simulator::bin2dec_u(s_rd);

        int type = rec_instr(bin_code);
        
        #ifdef DEBUG
        std::cout << "; type = " << type << "; i_rs = " << i_rs << "; i_rt = " << i_rt << 
            "; i_rd = " << i_rd << "; s_imm = " << mips_simulator::bin2dec_u(s_imm) << "; pc = " << rgt_pc << std::endl;
        #endif

        exe_funct(type, pt, i_rs, i_rt, i_rd, 
                  s_shamt, s_imm, s_add, memory, 
                  syscall_inputs, output_file);


        #ifdef DEBUG
        show_registers();
        std::cout << std::endl;
        #endif

        rgt_pc += 4;
    }
}


void Simulator::exe_funct(int type, uint32_t*& pt, uint32_t i_rs, uint32_t i_rt, uint32_t i_rd, 
                          std::string s_shamt, std::string s_imm, std::string s_add, uint32_t* memory, 
                          std::ifstream& syscall_inputs, std::ofstream& output_file)
{

    if (type >= 0 && type <= 10)
    { // r_dst
        R_dst_functions[type](&rgt[i_rd], &rgt[i_rs], &rgt[i_rt]);
    }
    else if (type >= 11 && type <= 12)
    { // r_ds ...
        R_ds_function[type - 11](&rgt[i_rd], &rgt[i_rs]);
    }
    else if (type >= 13 && type <= 20)
    { // r_sthl
        R_sthl_function[type - 13](&rgt[i_rs], &rgt[i_rt], &hi, &lo);
    }
    else if (type >= 21 && type <= 26)
    { // r_stt
        R_stt_function[type - 21](&rgt[i_rs], &rgt[i_rt]);
    }
    else if (type >= 27 && type <= 29)
    { // r_dtshamt
        R_dtshamt_function[type - 27](&rgt[i_rd], &rgt[i_rt], s_shamt);
    }
    else if (type == 30)
    { // r_sd
        mips_funct::JalrF(&rgt[i_rs], &rgt[i_rd], &rgt_pc);
    }
    else if (type == 31)
    { // r_s_jr
        int jrf = mips_funct::JrF(&rgt[i_rs], &rgt_pc);
        pt = pt + jrf / 4 - 1;
        rgt_pc -= 4;
    }
    else if (type == 32)
    { // r_s_hi
        mips_funct::MthiF(&rgt[i_rs], &hi);
    }
    else if (type == 33)
    { // r_s_lo
        mips_funct::MtloF(&rgt[i_rs], &lo);
    }
    else if (type == 34)
    { // r_d_hi
        mips_funct::MfhiF(&rgt[i_rd], &hi);
    }
    else if (type == 35)
    { // r_d_lo
        mips_funct::MfloF(&rgt[i_rd], &lo);
    }
    else if (type >= 36 && type <= 38)
    { // r_dts
        R_dts_function[type - 36](&rgt[i_rd], &rgt[i_rt], &rgt[i_rs]);
    }
    else if (type == 39)
    {                                                                                                           // syscall
        mips_funct::SyscallF(&rgt[2], &rgt[4], &rgt[5], &rgt[6], memory, &dm_end, syscall_inputs, output_file); // dynamic byte
    }
    else if (type >= 40 && type <= 46)
    { // i_tsi
        I_tsi_function[type - 40](&rgt[i_rt], &rgt[i_rs], s_imm);
    }
    else if (type == 47)
    { // i_ti
        mips_funct::LuiF(&rgt[i_rt], s_imm);
    }
    else if (type >= 48 && type <= 49)
    { // i_stl
        int stl = I_stl_function[type - 48](&rgt[i_rs], &rgt[i_rt], &rgt_pc, s_imm);
        pt = pt + stl / 4 - 1;
        if (stl != 4)
        {
            rgt_pc -= 4;
        }
    }
    else if (type >= 50 && type <= 55)
    { // i_sl
        int sl = I_sl_function[type - 50](&rgt[i_rs], &rgt_pc, s_imm, &rgt[31]);
        pt = pt + sl / 4 - 1;
        if (sl != 4)
        {
            rgt_pc -= 4;
        }
    }
    else if (type >= 56 && type <= 61)
    { // i_si
        I_si_function[type - 56](&rgt[i_rs], s_imm);
    }
    else if (type >= 62 && type <= 75)
    { // i_ta
        I_ta_function[type - 62](&rgt[i_rs], &rgt[i_rt], s_imm, memory);
    }
    else if (type == 76)
    { // j
        int j = mips_funct::JF(&rgt_pc, s_add);
        pt = pt + j / 4 - 1;
        rgt_pc -= 4;
    }
    else if (type == 77)
    { // jal
        int jal = mips_funct::JalF(&rgt_pc, s_add, &rgt[31]);
        pt = pt + jal / 4 - 1;
        rgt_pc -= 4;
    }
}

int Simulator::rec_instr(std::string code)
{
    int rij = 0;
    std::string opcode = code.substr(0, 6);

    if (opcode == "000000")
    { // r 000000
        rij = 1;
    }
    else if (opcode == "011100")
    { // r 011100
        rij = 2;
    }
    else if (opcode == "000010" || opcode == "000011")
    { // j
        rij = 3;
    }
    else
    { // i
        rij = 4;
    }

    int func_type;
    std::string funct = code.substr(26, 6);

    switch (rij)
    {
    case 1:
    { // r 000000
        if (funct == "100000")
        { // add
            func_type = 0;
            #ifdef DEBUG
            std::cout << "add ";
            #endif
        }
        else if (funct == "100001")
        { // addu
            func_type = 1;
            #ifdef DEBUG
            std::cout << "addu ";
            #endif
        }
        else if (funct == "100100")
        { // and
            func_type = 2;
            #ifdef DEBUG
            std::cout << "and ";
            #endif
        }
        else if (funct == "011010")
        { // div
            func_type = 13;
            #ifdef DEBUG
            std::cout << "div ";
            #endif
        }
        else if (funct == "011011")
        { // divu
            func_type = 14;
            #ifdef DEBUG
            std::cout << "divu ";
            #endif
        }
        else if (funct == "011000")
        { // mult
            func_type = 15;
            #ifdef DEBUG
            std::cout << "mult ";
            #endif
        }
        else if (funct == "011001")
        { // multu
            func_type = 16;
            #ifdef DEBUG
            std::cout << "multu ";
            #endif
        }
        else if (funct == "100111")
        { // nor
            func_type = 4;
            #ifdef DEBUG
            std::cout << "nor ";
            #endif
        }
        else if (funct == "100101")
        { // or
            func_type = 5;
            #ifdef DEBUG
            std::cout << "or ";
            #endif
        }
        else if (funct == "000000")
        { // sll
            func_type = 27;
            #ifdef DEBUG
            std::cout << "sll ";
            #endif
        }
        else if (funct == "000100")
        { // sllv
            func_type = 36;
            #ifdef DEBUG
            std::cout << "sllv ";
            #endif
        }
        else if (funct == "000011")
        { // sra
            func_type = 28;
            #ifdef DEBUG
            std::cout << "sra ";
            #endif
        }
        else if (funct == "000111")
        { // srav
            func_type = 37;
            #ifdef DEBUG
            std::cout << "srav ";
            #endif
        }
        else if (funct == "000010")
        { // srl
            func_type = 29;
            #ifdef DEBUG
            std::cout << "srl ";
            #endif
        }
        else if (funct == "000110")
        { // srlv
            func_type = 38;
            #ifdef DEBUG
            std::cout << "srlv ";
            #endif
        }
        else if (funct == "100010")
        { // sub
            func_type = 6;
            #ifdef DEBUG
            std::cout << "sub ";
            #endif
        }
        else if (funct == "100011")
        { // subu
            func_type = 7;
            #ifdef DEBUG
            std::cout << "subu ";
            #endif
        }
        else if (funct == "100110")
        { // xor
            func_type = 8;
            #ifdef DEBUG
            std::cout << "xor ";
            #endif
        }
        else if (funct == "101010")
        { // slt
            func_type = 9;
            #ifdef DEBUG
            std::cout << "slt ";
            #endif
        }
        else if (funct == "101011")
        { // sltu
            func_type = 10;
            #ifdef DEBUG
            std::cout << "sltu ";
            #endif
        }
        else if (funct == "001001")
        { // jalr
            func_type = 30;
            #ifdef DEBUG
            std::cout << "jalr ";
            #endif
        }
        else if (funct == "001000")
        { // jr
            func_type = 31;
            #ifdef DEBUG
            std::cout << "jr ";
            #endif
        }
        else if (funct == "110100")
        { // teq
            func_type = 21;
            #ifdef DEBUG
            std::cout << "teq ";
            #endif
        }
        else if (funct == "110110")
        { // tne
            func_type = 22;
            #ifdef DEBUG
            std::cout << "tne ";
            #endif
        }
        else if (funct == "110000")
        { // tge
            func_type = 23;
            #ifdef DEBUG
            std::cout << "tge ";
            #endif
        }
        else if (funct == "110001")
        { // tgeu
            func_type = 24;
            #ifdef DEBUG
            std::cout << "tgeu ";
            #endif
        }
        else if (funct == "110010")
        { // tlt
            func_type = 25;
        }
        else if (funct == "110011")
        { // tltu
            func_type = 26;
        }
        else if (funct == "010000")
        { // mfhi
            func_type = 34;
        }
        else if (funct == "010010")
        { // mflo
            func_type = 35;
        }
        else if (funct == "010001")
        { // mthi
            func_type = 32;
        }
        else if (funct == "010010")
        { // mtlo
            func_type = 33;
        }
        else if (funct == "001100")
        { // syscall
            func_type = 39;
            #ifdef DEBUG
            std::cout << "syscall ";
            #endif
        }
        break;
    }
    case 2:
    { // r 011100
        if (funct == "100001")
        { // clo
            func_type = 11;
        }
        else if (funct == "100000")
        { // clz
            func_type = 12;
        }
        else if (funct == "000010")
        { // mul
            func_type = 3;
        }
        else if (funct == "000000")
        { // madd
            func_type = 17;
        }
        else if (funct == "000001")
        { // maddu
            func_type = 18;
        }
        else if (funct == "000100")
        { // msub
            func_type = 19;
        }
        else if (funct == "000101")
        { // msubu
            func_type = 20;
        }
        break;
    }
    case 3:
    { // j
        if (opcode == "000010")
        { // j
            func_type = 76;
            #ifdef DEBUG
            std::cout << "j ";
            #endif
        }
        else if (opcode == "000011")
        { // jal
            func_type = 77;
            #ifdef DEBUG
            std::cout << "jal ";
            #endif
        }
        break;
    }
    case 4: // i
    {

        if (opcode == "001000")
        { // addi
            func_type = 40;
            #ifdef DEBUG
            std::cout << "addi ";
            #endif
        }
        else if (opcode == "001001")
        { // addiu
            func_type = 41;
            #ifdef DEBUG
            std::cout << "addiu ";
            #endif
        }
        else if (opcode == "001100")
        { // andi
            func_type = 42;
            #ifdef DEBUG
            std::cout << "andi ";
            #endif
        }
        else if (opcode == "001101")
        { // ori
            func_type = 43;
            #ifdef DEBUG
            std::cout << "ori ";
            #endif
        }
        else if (opcode == "001110")
        { // xori
            func_type = 44;
        }
        else if (opcode == "001111")
        { // lui
            func_type = 47;
            #ifdef DEBUG
            std::cout << "lui ";
            #endif
        }
        else if (opcode == "001010")
        { // slti
            func_type = 45;
        }
        else if (opcode == "001011")
        { // sltiu
            func_type = 46;
        }
        else if (opcode == "000100")
        { // beq
            func_type = 48;
            #ifdef DEBUG
            std::cout << "beq ";
            #endif
        }
        else if (opcode == "000111")
        { // bgtz
            func_type = 52;
        }
        else if (opcode == "000110")
        { // blez
            func_type = 53;
            #ifdef DEBUG
            std::cout << "blez ";
            #endif
        }
        else if (opcode == "000101")
        { // bne
            func_type = 49;
        }
        else if (opcode == "100000")
        { // lb
            func_type = 62;
        }
        else if (opcode == "100100")
        { // lbu
            func_type = 63;
            #ifdef DEBUG
            std::cout << "lbu ";
            #endif
        }
        else if (opcode == "100001")
        { // lh
            func_type = 64;
        }
        else if (opcode == "100101")
        { // lhu
            func_type = 65;
        }
        else if (opcode == "100011")
        { // lw
            func_type = 66;
            #ifdef DEBUG
            std::cout << "lw ";
            #endif

        }
        else if (opcode == "100010")
        { // lwl
            func_type = 67;
        }
        else if (opcode == "100110")
        { // lwr
            func_type = 68;
        }
        else if (opcode == "110000")
        { // ll
            func_type = 69;
        }
        else if (opcode == "101000")
        { // sb
            func_type = 70;
        }
        else if (opcode == "101001")
        { // sh
            func_type = 71;
        }
        else if (opcode == "101011")
        { // sw
            func_type = 72;
            #ifdef DEBUG
            std::cout << "sw ";
            #endif
        }
        else if (opcode == "101010")
        { // swl
            func_type = 73;
        }
        else if (opcode == "101110")
        { // swr
            func_type = 74;
        }
        else if (opcode == "111000")
        { // sc
            func_type = 75;
        }
        else if (opcode == "000001")
        {
            std::string rt = code.substr(11, 5);
            if (rt == "00001")
            { // bgez
                func_type = 50;
            }
            else if (rt == "10001")
            { // bgezal
                func_type = 51;
            }
            else if (rt == "10000")
            { // bltzal
                func_type = 54;
            }
            else if (rt == "00000")
            { // bltz
                func_type = 55;
            }
            else if (rt == "01100")
            { // teqi
                func_type = 56;
            }
            else if (rt == "01110")
            { // tnei
                func_type = 57;
            }
            else if (rt == "01000")
            { // tgei
                func_type = 58;
            }
            else if (rt == "01001")
            { // tgeiu
                func_type = 59;
            }
            else if (rt == "01010")
            { // tlti
                func_type = 60;
            }
            else if (rt == "01011")
            { // tltiu
                func_type = 61;
            }
        }
        break;
    }
    default:;
    }

    return func_type;
}

void Simulator::show_registers()
{
    std::cout << "$zero = " << rgt[0] << std::endl;
    std::cout << "rgt[1]   " <<  "$at = " << rgt[1] << std::endl;
    std::cout << "rgt[2]   " <<  "$v0 = " << rgt[2] << std::endl;
    std::cout << "rgt[3]   " <<  "$v1 = " << rgt[3] << std::endl;
    std::cout << "rgt[4]   " <<  "$a0 = " << rgt[4] << std::endl;
    std::cout << "rgt[5]   " <<  "$a1 = " << rgt[5] << std::endl;
    std::cout << "rgt[6]   " <<  "$a2 = " << rgt[6] << std::endl;
    std::cout << "rgt[7]   " <<  "$a3 = " << rgt[7] << std::endl;
    std::cout << "rgt[8]   " <<  "$t0 = " << rgt[8] << std::endl;
    std::cout << "rgt[9]   " <<  "$t1 = " << rgt[9] << std::endl;
    std::cout << "rgt[10]  "  << "$t2 = " << rgt[10] << std::endl;
    std::cout << "rgt[11]  "  << "$t3 = " << rgt[11] << std::endl;
    std::cout << "rgt[12]  "  << "$t4 = " << rgt[12] << std::endl;
    std::cout << "rgt[13]  "  << "$t5 = " << rgt[13] << std::endl;
    std::cout << "rgt[14]  "  << "$t6 = " << rgt[14] << std::endl;
    std::cout << "rgt[15]  "  << "$t7 = " << rgt[15] << std::endl;
    std::cout << "rgt[16]  "  << "$s0 = " << rgt[16] << std::endl;
    std::cout << "rgt[17]  "  << "$s1 = " << rgt[17] << std::endl;
    std::cout << "rgt[18]  "  << "$s2 = " << rgt[18] << std::endl;
    std::cout << "rgt[19]  "  << "$s3 = " << rgt[19] << std::endl;
    std::cout << "rgt[20]  "  << "$s4 = " << rgt[20] << std::endl;
    std::cout << "rgt[21]  "  << "$s5 = " << rgt[21] << std::endl;
    std::cout << "rgt[22]  "  << "$s6 = " << rgt[22] << std::endl;
    std::cout << "rgt[23]  "  << "$s7 = " << rgt[23] << std::endl;
    std::cout << "rgt[24]  "  << "$t8 = " << rgt[24] << std::endl;
    std::cout << "rgt[25]  "  << "$t9 = " << rgt[25] << std::endl;
    std::cout << "rgt[26]  "  << "$k0 = " << rgt[26] << std::endl;
    std::cout << "rgt[27]  "  << "$k1 = " << rgt[27] << std::endl;
    std::cout << "rgt[28]  "  << "$gp = " << rgt[28] << std::endl;
    std::cout << "rgt[29]  "  << "$sp = " << rgt[29] << std::endl;
    std::cout << "rgt[30]  "  << "$fp = " << rgt[30] << std::endl;
    std::cout << "rgt[31]  "  << "$ra = " << rgt[31] << std::endl;
}

