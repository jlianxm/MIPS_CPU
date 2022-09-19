// This file contains util functions

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <sstream>
#include <math.h>

namespace mips_simulator
{
    // Segment string into tokens, split by " "
    std::vector<std::string> tokenizer(std::string str);

    // unsigned int -> machine code
    std::string dec2bin_u(std::string s_udec, int imm);

    // signed int -> imm
    std::string dec2bin(std::string s_dec, int imm);

    // change binary code to unsigned integers
    // usually for storing
    uint32_t bin2dec_u(std::string bin);

    // change binary code to signed integers
    // usually for calculation
    int bin2dec_i(std::string bin);

    std::string change_order_32(std::string code);

    // code: instruction.mcode
    // code: 44444444/33333333/22222222/11111111
    // reverse order
    // used to store the machine code
    // should revise the name: str2uint
    uint32_t change2uint(std::string code);

    // bin code
    uint32_t change2uint_without_reverse(std::string code);

    // change the uint32_t in registers to signed integer
    // input string format uint32_t
    int change2int(std::string code);

    std::string change_order_24(std::string code);

    std::string change_order_16(std::string code);

} // namespace mips_simulator

#endif