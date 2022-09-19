// This header file contains maps between opcode/registers/funct and its corresponding binary code
#ifndef DIC_H_
#define DIC_H_

#include <map>
#include <string>

namespace opcode_dic
{

    // Dictionary of the opcode of R type instruction
    std::map<std::string,std::string> op_r_dic {
        {"add","000000"},{"addu","000000"},{"and","000000"},{"clo","011100"},{"clz","011100"},{"div","000000"},{"divu","000000"},
        {"mult","000000"},{"multu","000000"},{"mul","011100"},{"madd","011100"},{"maddu","011100"},{"msub","011100"},{"msubu","011100"},
        {"nor","000000"},{"or","000000"},{"sll","000000"},{"sllv","000000"},{"sra","000000"},{"srav","000000"},{"srl","000000"},
        {"srlv","000000"},{"sub","000000"},{"subu","000000"},{"xor","000000"},{"slt","000000"},{"sltu","000000"},{"jalr","000000"},
        {"jr","000000"},{"teq","000000"},{"tne","000000"},{"tge","000000"},{"tgeu","000000"},{"tlt","000000"},{"tltu","000000"},
        {"mfhi","000000"},{"mflo","000000"},{"mthi","000000"},{"mtlo","000000"},{"syscall","000000"}
    };

    // Dictionary of the opcode of I type instruction
    std::map<std::string,std::string> op_i_dic {
        {"addi","001000"},{"addiu","001001"},{"andi","001100"},{"ori","001101"},{"xori","001110"},{"lui","001111"},{"slti","001010"},
        {"sltiu","001011"},{"beq","000100"},{"bgez","000001"},{"bgezal","000001"},{"bgtz","000111"},{"blez","000110"},{"bltzal","000001"},
        {"bltz","000001"},{"bne","000101"},{"teqi","000001"},{"tnei","000001"},{"tgei","000001"},{"tgeiu","000001"},{"tlti","000001"},
        {"tltiu","000001"},{"lb","100000"},{"lbu","100100"},{"lh","100001"},{"lhu","100101"},{"lw","100011"},{"lwl","100010"},{"lwr","100110"},
        {"ll","110000"},{"sb","101000"},{"sh","101001"},{"sw","101011"},{"swl","101010"},{"swr","101110"},{"sc","111000"}
    };


    // Dictionary of the opcode of J type instruction
    std::map<std::string,std::string> op_j_dic {
        {"j","000010"},{"jal","000011"}
    };

    // Dictionary of R type instruction, with rd, rs, rt
    std::map<std::string,std::string> r_dst {
        {"add","000000"},{"addu","000000"},{"and","000000"},{"mul","011100"},{"nor","000000"},{"or","000000"},
        {"sub","000000"},{"subu","000000"},{"xor","000000"},{"slt","000000"},{"sltu","000000"}
    };

    // Dictionary of R type instruction, with rd, rs
    std::map<std::string,std::string> r_ds {
        {"clo","011100"},{"clz","011100"}
    };

    // Dictionary of R type instruction, with rs, rt
    std::map<std::string,std::string> r_st {
        {"div","000000"},{"divu","000000"},{"mult","000000"},{"multu","000000"},{"madd","011100"},{"maddu","011100"},{"msub","011100"},
        {"msubu","011100"},{"teq","000000"},{"tne","000000"},{"tge","000000"},{"tgeu","000000"},{"tlt","000000"},{"tltu","000000"}
    };

    // Dictionary of R type instruction, with rs, rd, sa
    std::map<std::string,std::string> r_dtshamt {
        {"sll","000000"},{"sra","000000"},{"srl","000000"}
    };

    // Dictionary of R type instruction, with rs, rd
    std::map<std::string,std::string> r_sd {
        {"jalr","000000"}
    };

    // Dictionary of R type instruction, with rs
    std::map<std::string,std::string> r_s {
        {"jr","000000"},{"mthi","000000"},{"mtlo","000000"}
    };

    // Dictionary of R type instruction, with rd
    std::map<std::string,std::string> r_d{
        {"mfhi","000000"},{"mflo","000000"},
    };

    // Dictionary of R type instruction, with rd, rt, rs
    std::map<std::string,std::string> r_dts {
        {"sllv","000000"},{"srav","000000"},{"srlv","000000"}
    };

    // system call
    std::map<std::string,std::string> r_syscall {
        {"syscall","000000"}
    };

    // Dictionary of I type instruction, with rt, rs, immediate
    std::map<std::string,std::string> i_tsi{
        {"addi","001000"},{"addiu","001001"},{"andi","001100"},{"ori","001101"},{"xori","001110"},{"slti","001010"},{"sltiu","001011"}
    };

    // Dictionary of I type instruction, with rt, immediate
    std::map<std::string,std::string> i_ti{
        {"lui","001111"}
    };

    // Dictionary of I type instruction, with rs, rt, offset
    std::map<std::string,std::string> i_stl{
        {"beq","000100"},
        {"bne","000101"}
    };

    // Dictionary of I type instruction, with rs, offset
    std::map<std::string,std::string> i_sl{
        {"bgez","000001"},{"bgezal","000001"},{"bgtz","000111"},{"blez","000110"},{"bltzal","000001"},{"bltz","000001"}
    };

    // Dictionary of I type instruction, with rs, immediate
    std::map<std::string,std::string> i_si{
        {"teqi","000001"},{"tnei","000001"},{"tgei","000001"},{"tgeiu","000001"},{"tlti","000001"},{"tltiu","000001"}
    };

    // Dictionary of I type instruction, with rs, immediate
    std::map<std::string,std::string> i_ta{
        {"lb","100000"},{"lbu","100100"},{"lh","100001"},{"lhu","100101"},{"lw","100011"},{"lwl","100010"},{"lwr","100110"},
        {"ll","110000"},{"sb","101000"},{"sh","101001"},{"sw","101011"},{"swl","101010"},{"swr","101110"},{"sc","111000"}
    };

}

namespace rgt_dic
{
    // Dictionary of registers
    std::map<std::string,std::string> rgt_dic{
        {"$zero","00000"},{"$at","00001"},{"$v0","00010"},{"$v1","00011"},{"$a0","00100"},{"$a1","00101"},{"$a2","00110"},
        {"$a3","00111"},{"$t0","01000"},{"$t1","01001"},{"$t2","01010"},{"$t3","01011"},{"$t4","01100"},{"$t5","01101"},
        {"$t6","01110"},{"$t7","01111"},{"$s0","10000"},{"$s1","10001"},{"$s2","10010"},{"$s3","10011"},{"$s4","10100"},
        {"$s5","10101"},{"$s6","10110"},{"$s7","10111"},{"$t8","11000"},{"$t9","11001"},{"$k0","11010"},{"$k1","11011"},
        {"$gp","11100"},{"$sp","11101"},{"$fp","11110"},{"$ra","11111"}
    };
}


namespace r_funct_dic
{
    // Dictionary of the funct of R type instructions
    std::map<std::string,std::string> r_funct{
        {"add","100000"},{"addu","100001"},{"and","100100"},{"clo","100001"},{"clz","100000"},{"div","011010"},{"divu","011011"},
        {"mult","011000"},{"multu","011001"},{"mul","000010"},{"madd","000000"},{"maddu","000001"},{"msub","000100"},{"msubu","000101"},
        {"nor","100111"},{"or","100101"},{"sll","000000"},{"sllv","000100"},{"sra","000011"},{"srav","000111"},{"srl","000010"},
        {"srlv","000110"},{"sub","100010"},{"subu","100011"},{"xor","100110"},{"slt","101010"},{"sltu","101011"},{"jalr","001001"},
        {"jr","001000"},{"teq","110100"},{"tne","110110"},{"tge","110000"},{"tgeu","110001"},{"tlt","110010"},{"tltu","110011"},
        {"mfhi","010000"},{"mflo","010010"},{"mthi","010001"},{"mtlo","010010"},{"syscall","001100"}
    };
}


namespace escape_word_dic
{
    std::map<char,std::string> escape_word_dic{
        {'a',"00000111"}, {'b', "00001000"}, {'f',"00001100"}, {'n',"00001010"}, {'r',"00001101"}, {'t',"00001001"}, {'v',"00001011"}, 
        {'\\',"01011100"}, {'\'',"00100111"}, {'\"',"00100010"}, {'?',"00111111"}, {'0',"00000000"}
    };
    
} // namespace escape_word_dic


#endif
