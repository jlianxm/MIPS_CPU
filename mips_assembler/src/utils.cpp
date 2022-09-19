#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include "utils.h"

// Segment string into tokens, split by " "
std::vector<std::string> mips_simulator::tokenizer(std::string str)
{
    std::stringstream ss(str);
    std::vector<std::string> tokens;
    std::string word;
    while (ss >> word)
    {
        tokens.push_back(word);
    }
    ss.clear();
    return (tokens);
}

// unsigned int -> machine code
std::string mips_simulator::dec2bin_u(std::string s_udec, int imm)
{
    std::istringstream is(s_udec);
    uint32_t i_dec;
    int len;
    std::string s_res;
    is >> i_dec;
    uint32_t temp = i_dec;
    while (temp)
    {
        s_res = std::to_string(temp % 2) + s_res;
        temp = temp / 2;
    }
    if (i_dec >= 0)
    {
        len = s_res.length();
        for (int i = 0; i < imm - len; i++)
        {
            s_res = "0" + s_res;
        }
    }
    return s_res;
}

// signed int -> imm
std::string mips_simulator::dec2bin(std::string s_dec, int imm)
{ // imm
    std::istringstream is(s_dec);
    int i_dec;
    int len;
    std::string s_res;
    is >> i_dec;
    int temp = i_dec;
    if (temp < 0)
    {
        temp = -temp;
    }
    while (temp)
    {
        s_res = std::to_string(temp % 2) + s_res;
        temp = temp / 2;
    }
    if (i_dec >= 0)
    {
        len = s_res.length();
        for (int i = 0; i < imm - len; i++)
        {
            s_res = "0" + s_res;
        }
    }
    else
    {
        len = s_res.length();
        for (int i = 0; i < imm - len; i++)
        {
            s_res = "0" + s_res;
        }
        for (std::string::iterator iter = s_res.begin(); iter != s_res.end(); iter++)
        {
            if (*iter == '1')
            {
                *iter = '0';
            }
            else
            {
                *iter = '1';
            }
        }
        for (std::string::reverse_iterator iter = s_res.rbegin(); iter != s_res.rend(); iter++)
        {
            if (*iter == '1')
            {
                *iter = '0';
            }
            else
            {
                *iter = '1';
                break;
            }
        }
    }
    return s_res;
}

// change binary code to unsigned integers
// usually for storing
uint32_t mips_simulator::bin2dec_u(std::string bin)
{
    int len = bin.length();
    uint32_t res = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        if (bin[i] == '1')
        {
            res += pow(2.0, len - i - 1);
        }
    }
    return res;
}

// change binary code to signed integers
// usually for calculation
int mips_simulator::bin2dec_i(std::string bin)
{
    int len = bin.length();
    int res = 0;
    for (int i = len - 1; i >= 1; i--)
    {
        if (bin[i] == '1')
        {
            res += pow(2.0, len - i - 1);
        }
    }
    if (bin[0] == '1')
    {
        res -= pow(2.0, len - 1);
    }
    return res;
}

std::string mips_simulator::change_order_32(std::string code)
{
    std::string s_code4 = code.substr(0, 8);
    std::string s_code3 = code.substr(8, 8);
    std::string s_code2 = code.substr(16, 8);
    std::string s_code1 = code.substr(24, 8);
    std::string reversed_code_b = s_code1 + s_code2 + s_code3 + s_code4;
    return reversed_code_b;
}

// code: instruction.mcode
// code: 44444444/33333333/22222222/11111111
// reverse order
// used to store the machine code
// should revise the name: str2uint
uint32_t mips_simulator::change2uint(std::string code)
{
    std::string reversed_code_b = change_order_32(code);
    uint32_t ui_r_code = bin2dec_u(reversed_code_b);
    return ui_r_code;
}

// bin code
uint32_t mips_simulator::change2uint_without_reverse(std::string code)
{
    uint32_t ui_code = bin2dec_u(code);
    return ui_code;
}

// change the uint32_t in registers to signed integer
// input string format uint32_t
int mips_simulator::change2int(std::string code)
{
    int i_rd = bin2dec_i(dec2bin_u(code, 32));
    return i_rd;
}

std::string mips_simulator::change_order_24(std::string code)
{
    std::string s_code4 = code.substr(0, 8);
    std::string s_code3 = code.substr(8, 8);
    std::string s_code2 = code.substr(16, 8);
    std::string reversed_code_b = s_code2 + s_code3 + s_code4;
    return reversed_code_b;
}

std::string mips_simulator::change_order_16(std::string code)
{
    std::string s_code2 = code.substr(0, 8);
    std::string s_code1 = code.substr(8, 8);
    std::string reversed_code_b = s_code1 + s_code2;
    return reversed_code_b;
}