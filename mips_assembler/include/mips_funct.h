#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdint.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include "utils.h"

namespace mips_funct
{

    // r_dst
    void AddF(uint32_t *rd, uint32_t *rs, uint32_t *rt)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_rt = mips_simulator::change2int(std::to_string(*rt));
        long long ll_rs = (long long)i_rs;
        long long ll_rt = (long long)i_rt;
        long long ll_res = ll_rs + ll_rt;
        if (ll_res > 2147483647 || ll_res < -2147483648)
        {
            std::cout << "overflow" << std::endl;
            exit(1);
        }
        else
        {
            int i_rd = (int)ll_res;
            *rd = mips_simulator::change2uint_without_reverse(mips_simulator::dec2bin(std::to_string(i_rd), 32));
        }
    }

    void AdduF(uint32_t *rd, uint32_t *rs, uint32_t *rt)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_rt = mips_simulator::change2int(std::to_string(*rt));
        int i_rd = i_rs + i_rt;
        *rd = mips_simulator::change2uint_without_reverse(mips_simulator::dec2bin(std::to_string(i_rd), 32));
    }

    void AndF(uint32_t *rd, uint32_t *rs, uint32_t *rt)
    {
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        std::string s_rs = mips_simulator::dec2bin_u(std::to_string(*rs), 32);
        std::string s_rd = "00000000000000000000000000000000";
        for (int i = 0; i < 32; i++)
        {
            if (s_rt[i] == '1' && s_rs[i] == '1')
            {
                s_rd[i] = '1';
            }
        }
        *rd = mips_simulator::bin2dec_u(s_rd);
    }

    void MulF(uint32_t *rd, uint32_t *rs, uint32_t *rt)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_rt = mips_simulator::change2int(std::to_string(*rt));
        long long ll_res = i_rs * i_rt;
        std::string res_64 = mips_simulator::dec2bin(std::to_string(ll_res), 64);
        std::string res_32 = res_64.substr(32, 32);
        *rd = mips_simulator::bin2dec_u(res_32);
    }

    void NorF(uint32_t *rd, uint32_t *rs, uint32_t *rt)
    {
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        std::string s_rs = mips_simulator::dec2bin_u(std::to_string(*rs), 32);
        std::string s_rd = "00000000000000000000000000000000";
        for (int i = 0; i < 32; i++)
        {
            if (s_rt[i] == '0' && s_rs[i] == '0')
            {
                s_rd[i] = '1';
            }
        }
        *rd = mips_simulator::bin2dec_u(s_rd);
    }

    void OrF(uint32_t *rd, uint32_t *rs, uint32_t *rt)
    {
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        std::string s_rs = mips_simulator::dec2bin_u(std::to_string(*rs), 32);
        std::string s_rd = "00000000000000000000000000000000";
        for (int i = 0; i < 32; i++)
        {
            if (s_rt[i] == '1' || s_rs[i] == '1')
            {
                s_rd[i] = '1';
            }
        }
        *rd = mips_simulator::bin2dec_u(s_rd);
    }

    void SubF(uint32_t *rd, uint32_t *rs, uint32_t *rt)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_rt = mips_simulator::change2int(std::to_string(*rt));
        long long ll_rs = (long long)i_rs;
        long long ll_rt = (long long)i_rt;
        long long ll_res = ll_rs - ll_rt;
        if (ll_res > 2147483647 || ll_res < -2147483648)
        {
            std::cout << "overflow" << std::endl;
            exit(1);
        }
        else
        {
            int i_rd = (int)ll_res;
            *rd = mips_simulator::change2uint_without_reverse(mips_simulator::dec2bin(std::to_string(i_rd), 32));
        }
    }

    void SubuF(uint32_t *rd, uint32_t *rs, uint32_t *rt)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_rt = mips_simulator::change2int(std::to_string(*rt));
        int i_rd = i_rs - i_rt;
        *rd = mips_simulator::change2uint_without_reverse(mips_simulator::dec2bin(std::to_string(i_rd), 32));
    }

    void XorF(uint32_t *rd, uint32_t *rs, uint32_t *rt)
    {
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        std::string s_rs = mips_simulator::dec2bin_u(std::to_string(*rs), 32);
        std::string s_rd = "00000000000000000000000000000000";
        for (int i = 0; i < 32; i++)
        {
            if (s_rt[i] != s_rs[i])
            {
                s_rd[i] = '1';
            }
        }
        *rd = mips_simulator::bin2dec_u(s_rd);
    }

    void SltF(uint32_t *rd, uint32_t *rs, uint32_t *rt)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_rt = mips_simulator::change2int(std::to_string(*rt));
        if (i_rs < i_rt)
        {
            *rd = 1;
        }
        else
        {
            *rd = 0;
        }
    }

    void SltuF(uint32_t *rd, uint32_t *rs, uint32_t *rt)
    {
        if (*rs < *rt)
        {
            *rd = 1;
        }
        else
        {
            *rd = 0;
        }
    }

    // r_ds
    void CloF(uint32_t *rd, uint32_t *rs)
    {
        std::string s_rs = mips_simulator::dec2bin_u(std::to_string(*rs), 32);
        uint32_t num = 32;
        for (int i = 0; i < 32; i++)
        {
            if (s_rs[i] == '0')
            {
                num = i;
                break;
            }
        }
        *rd = num;
    }

    void ClzF(uint32_t *rd, uint32_t *rs)
    {
        std::string s_rs = mips_simulator::dec2bin_u(std::to_string(*rs), 32);
        uint32_t num = 32;
        for (int i = 0; i < 32; i++)
        {
            if (s_rs[i] == '1')
            {
                num = i;
                break;
            }
        }
        *rd = num;
    }

    // r_sthl
    void DivF(uint32_t *rs, uint32_t *rt, uint32_t *hi, uint32_t *lo)
    { // the remainder is positive or negative?
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_rt = mips_simulator::change2int(std::to_string(*rt));
        int quo = i_rs / i_rt;
        int rem = i_rs % i_rt;
        *hi = mips_simulator::change2uint_without_reverse(mips_simulator::dec2bin(std::to_string(rem), 32));
        *lo = mips_simulator::change2uint_without_reverse(mips_simulator::dec2bin(std::to_string(quo), 32));
    }

    void DivuF(uint32_t *rs, uint32_t *rt, uint32_t *hi, uint32_t *lo)
    {
        uint32_t quo = *rs / *rt;
        uint32_t rem = *rs % *rt;
        *hi = rem;
        *lo = quo;
    }

    void MultF(uint32_t *rs, uint32_t *rt, uint32_t *hi, uint32_t *lo)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_rt = mips_simulator::change2int(std::to_string(*rt));
        long long ll_res = i_rs * i_rt;
        std::string res_64 = mips_simulator::dec2bin(std::to_string(ll_res), 64);
        std::string res_32_63 = res_64.substr(32, 32);
        std::string res_0_31 = res_64.substr(0, 32);
        *hi = mips_simulator::change2uint_without_reverse(res_32_63);
        *lo = mips_simulator::change2uint_without_reverse(res_0_31);
    }

    void MultuF(uint32_t *rs, uint32_t *rt, uint32_t *hi, uint32_t *lo)
    {
        long long ll_res = *rs * *rt;
        std::string res_64 = mips_simulator::dec2bin_u(std::to_string(ll_res), 64);
        std::string res_32_63 = res_64.substr(32, 32);
        std::string res_0_31 = res_64.substr(0, 32);
        *hi = mips_simulator::change2uint_without_reverse(res_32_63);
        *lo = mips_simulator::change2uint_without_reverse(res_0_31);
    }

    void MaddF(uint32_t *rs, uint32_t *rt, uint32_t *hi, uint32_t *lo)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_rt = mips_simulator::change2int(std::to_string(*rt));
        long long ll_res = i_rs * i_rt;
        std::string s_hi = mips_simulator::dec2bin_u(std::to_string(*hi), 32);
        std::string s_lo = mips_simulator::dec2bin_u(std::to_string(*lo), 32);
        std::string s_hilo = s_hi + s_lo;
        long long ll_hilo = mips_simulator::bin2dec_i(s_hilo);
        ll_res = ll_res + ll_hilo;
        std::string s_res = mips_simulator::dec2bin(std::to_string(ll_res), 64);
        s_hi = s_res.substr(32, 32);
        s_lo = s_res.substr(0, 32);
        *hi = mips_simulator::change2uint_without_reverse(s_hi);
        *lo = mips_simulator::change2uint_without_reverse(s_lo);
    }

    void MadduF(uint32_t *rs, uint32_t *rt, uint32_t *hi, uint32_t *lo)
    {
        uint64_t ll_res = *rs * *rt;
        std::string s_hi = mips_simulator::dec2bin_u(std::to_string(*hi), 32);
        std::string s_lo = mips_simulator::dec2bin_u(std::to_string(*lo), 32);
        std::string s_hilo = s_hi + s_lo;
        uint64_t ll_hilo = mips_simulator::bin2dec_u(s_hilo);
        ll_res = ll_res + ll_hilo;
        std::string s_res = mips_simulator::dec2bin_u(std::to_string(ll_res), 64);
        s_hi = s_res.substr(32, 32);
        s_lo = s_res.substr(0, 32);
        *hi = mips_simulator::change2uint_without_reverse(s_hi);
        *lo = mips_simulator::change2uint_without_reverse(s_lo);
    }

    void MsubF(uint32_t *rs, uint32_t *rt, uint32_t *hi, uint32_t *lo)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_rt = mips_simulator::change2int(std::to_string(*rt));
        long long ll_res = i_rs * i_rt;
        std::string s_hi = mips_simulator::dec2bin_u(std::to_string(*hi), 32);
        std::string s_lo = mips_simulator::dec2bin_u(std::to_string(*lo), 32);
        std::string s_hilo = s_hi + s_lo;
        long long ll_hilo = mips_simulator::bin2dec_i(s_hilo);
        ll_res = ll_hilo - ll_res;
        std::string s_res = mips_simulator::dec2bin(std::to_string(ll_res), 64);
        s_hi = s_res.substr(32, 32);
        s_lo = s_res.substr(0, 32);
        *hi = mips_simulator::change2uint_without_reverse(s_hi);
        *lo = mips_simulator::change2uint_without_reverse(s_lo);
    }

    void MsubuF(uint32_t *rs, uint32_t *rt, uint32_t *hi, uint32_t *lo)
    {
        uint64_t ll_res = *rs * *rt;
        std::string s_hi = mips_simulator::dec2bin_u(std::to_string(*hi), 32);
        std::string s_lo = mips_simulator::dec2bin_u(std::to_string(*lo), 32);
        std::string s_hilo = s_hi + s_lo;
        uint64_t ll_hilo = mips_simulator::bin2dec_u(s_hilo);
        ll_res = ll_hilo - ll_res;
        std::string s_res = mips_simulator::dec2bin_u(std::to_string(ll_res), 64);
        s_hi = s_res.substr(32, 32);
        s_lo = s_res.substr(0, 32);
        *hi = mips_simulator::change2uint_without_reverse(s_hi);
        *lo = mips_simulator::change2uint_without_reverse(s_lo);
    }

    // r_stt
    void TeqF(uint32_t *rs, uint32_t *rt)
    {
        if (*rs == *rt)
        {
            exit(1);
        }
    }

    void TneF(uint32_t *rs, uint32_t *rt)
    {
        if (*rs != *rt)
        {
            exit(1);
        }
    }

    void TgeF(uint32_t *rs, uint32_t *rt)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_rt = mips_simulator::change2int(std::to_string(*rt));
        if (i_rs >= i_rt)
        {
            exit(1);
        }
    }

    void TgeuF(uint32_t *rs, uint32_t *rt)
    {
        if (*rs >= *rt)
        {
            exit(1);
        }
    }

    void TltF(uint32_t *rs, uint32_t *rt)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_rt = mips_simulator::change2int(std::to_string(*rt));
        if (i_rs < i_rt)
        {
            exit(1);
        }
    }

    void TltuF(uint32_t *rs, uint32_t *rt)
    {
        if (*rs < *rt)
        {
            exit(1);
        }
    }

    // r_dtshamt
    void SllF(uint32_t *rd, uint32_t *rt, std::string shamt)
    {
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        int len = mips_simulator::bin2dec_u(shamt);
        for (int i = 0; i < len; i++)
        {
            s_rt = s_rt + '0';
        }
        std::string s_rd = s_rt.substr(len, 32);
        *rd = mips_simulator::change2uint_without_reverse(s_rd);
    }

    void SraF(uint32_t *rd, uint32_t *rt, std::string shamt)
    {
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        std::string rt_31 = s_rt.substr(0, 1);
        int len = mips_simulator::bin2dec_u(shamt);
        for (int i = 0; i < len; i++)
        {
            s_rt = rt_31 + s_rt;
        }
        std::string s_rd = s_rt.substr(0, 32);
        *rd = mips_simulator::change2uint_without_reverse(s_rd);
    }

    void SrlF(uint32_t *rd, uint32_t *rt, std::string shamt)
    {
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        int len = mips_simulator::bin2dec_u(shamt);
        for (int i = 0; i < len; i++)
        {
            s_rt = '0' + s_rt;
        }
        std::string s_rd = s_rt.substr(0, 32);
        *rd = mips_simulator::change2uint_without_reverse(s_rd);
    }

    // r_sd
    void JalrF(uint32_t *rs, uint32_t *rd, uint32_t *pc)
    {
        *rd = *pc + 4;
        *rs = *pc;
    }

    // r_s_jr
    int JrF(uint32_t *rs, uint32_t *pc)
    {
        uint32_t jr = *pc;
        *pc = *rs;
        uint32_t cur = *pc;
        return cur - jr;
    }

    // r_s_hi
    void MthiF(uint32_t *rs, uint32_t *hi)
    {
        *hi = *rs;
    }

    // r_s_lo
    void MtloF(uint32_t *rs, uint32_t *lo)
    {
        *lo = *rs;
    }

    // r_d_hi
    void MfhiF(uint32_t *rd, uint32_t *hi)
    {
        *rd = *hi;
    }

    // r_d_lo
    void MfloF(uint32_t *rd, uint32_t *lo)
    {
        *rd = *lo;
    }

    // r_dts
    void SllvF(uint32_t *rd, uint32_t *rt, uint32_t *rs)
    {
        uint32_t len = *rs;
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        for (uint32_t i = 0; i < len; i++)
        {
            s_rt = s_rt + '0';
        }
        std::string s_rd = s_rt.substr(len, 32);
        *rd = mips_simulator::change2uint_without_reverse(s_rd);
    }

    void SravF(uint32_t *rd, uint32_t *rt, uint32_t *rs)
    {
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        std::string rt_31 = s_rt.substr(0, 1);
        uint32_t len = *rs;
        for (uint32_t i = 0; i < len; i++)
        {
            s_rt = rt_31 + s_rt;
        }
        std::string s_rd = s_rt.substr(0, 32);
        *rd = mips_simulator::change2uint_without_reverse(s_rd);
    }

    void SrlvF(uint32_t *rd, uint32_t *rt, uint32_t *rs)
    {
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        uint32_t len = *rs;
        for (uint32_t i = 0; i < len; i++)
        {
            s_rt = '0' + s_rt;
        }
        std::string s_rd = s_rt.substr(0, 32);
        *rd = mips_simulator::change2uint_without_reverse(s_rd);
    }

    void SyscallF(uint32_t *v0, uint32_t *a0, uint32_t *a1, uint32_t *a2, uint32_t *memory, uint32_t *dynamic_end, std::ifstream &si, std::ofstream &o)
    {
        // std::cout << "calling SyscallF with *v0 = " << *v0 << std::endl;
        switch (*v0)
        {
        case 1:
        {   
            // std::cout << *a0 << std::endl;
            o << *a0;
            o.flush();
            break;
        }
        case 4:
        { // print string

            std::vector<int> v_word;
            uint32_t *str_start = memory + (*a0 / 4);
            bool read = true;
            int start = *a0 % 4;

            #ifdef DEBUG
            std::cout << "str_start = " << str_start << "; memory = " << memory << std::endl;
            std::cout << "str_start - memory = " << str_start - memory << std::endl;
            #endif

            while (read)
            {
                std::string word = mips_simulator::dec2bin_u(std::to_string(*str_start), 32);
                std::cout << "word = " << word << std::endl;
                for (int i = 8 * (3 - start); i >= 0; i = i - 8)
                {
                    std::string sin_word = word.substr(i, 8);
                    if (sin_word != "00000000")
                    {
                        v_word.push_back(mips_simulator::bin2dec_i(sin_word));
                    }
                    else
                    {
                        read = false;
                        break;
                    }
                }
                if (!read)
                {
                    break;
                }
                start = 0;
                str_start++;
            }

            for (std::vector<int>::iterator iter = v_word.begin(); iter != v_word.end(); iter++)
            {
                char c = *iter;
                o << c;
                o.flush();
                printf("%c",c);
            }
            break;
        }
        case 5:
        { // read int
            int v;
            std::string str;
            std::getline(si, str);
            std::stringstream ss;
            ss << str;
            ss >> v;
            *v0 = mips_simulator::change2uint_without_reverse(mips_simulator::dec2bin(std::to_string(v), 32));
            break;
        }
        case 8:
        { // read string
            int num_of_word = *a0 / 4;
            int num_of_byte = *a0 % 4;
            memory = memory + num_of_word;
            uint32_t ui_cur_word = *memory;
            std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);
            int len = *a1;
            std::string str = "";
            std::getline(si, str);
            uint8_t arr[str.length() + 1]; // buffer
            std::string s_sin_char;        // change a char to a byte
            uint8_t u_sin_char;
            for (uint32_t i = 0; i < str.length(); i++)
            {
                s_sin_char = mips_simulator::dec2bin_u(std::to_string(str[i]), 8);
                u_sin_char = mips_simulator::bin2dec_u(s_sin_char);
                arr[i] = u_sin_char;
            }
            if (str.length() < len - 1)
            {
                arr[str.length() - 1] = 10;
            }
            arr[str.length()] = 0;

            int j = 0;
            std::string byte;
            if (num_of_byte != 0)
            {
                for (int i = 8 * (num_of_byte); i < 32; i = i + 8)
                {
                    if (arr[j] != 0)
                    {
                        byte = mips_simulator::dec2bin_u(std::to_string(arr[j]), 8);
                        for (int k = 0; k < 8; k++)
                        {
                            s_cur_word[i + k] = byte[k];
                        }
                        j++;
                    }
                }
                len -= 4 - num_of_byte; // 包括\0
                *memory = mips_simulator::bin2dec_u(s_cur_word);
                memory++;
            }

            std::vector<uint8_t> vec_arr;
            for (int i = j; i < str.length() + 1; i++)
            {
                vec_arr.push_back(arr[j]);
            }
            std::vector<uint8_t>::iterator iter_arr = vec_arr.begin();
            int rest = vec_arr.size();
            num_of_word = rest / 4;
            num_of_byte = rest % 4;
            std::string word;

            for (int i = 0; i < num_of_word; i++)
            {
                for (int k = 0; k < 4; k++)
                {
                    byte = mips_simulator::dec2bin_u(std::to_string(*iter_arr), 8);
                    *iter_arr++;
                    word = word + byte;
                }
                *memory = mips_simulator::bin2dec_u(word);
                memory++;
                word = "";
            }

            for (int i = 0; i < num_of_byte; i++)
            {
                byte = byte = mips_simulator::dec2bin_u(std::to_string(*iter_arr), 8);
                word = word + byte;
            }
            *memory = mips_simulator::bin2dec_u(word);
            break;
        }
        case 9:
        { // sbrk
            *v0 = *dynamic_end;
            *dynamic_end += *a0;
            break;
        }
        case 10:
        { // exit
            exit(0);
            break;
        }
        case 11:
        {   // print char
            //        printf("%c",*a0);
            char c = *a0;
            o << c;
            o.flush();
            break;
        }
        case 12:
        { // read char
            char c;
            c = si.get();
            //        c = getchar();
            int ci = (int)c;
            *v0 = ci;
            break;
        }
        case 13:
        { // open file
            int a = mips_simulator::change2int(std::to_string(*a1));
            std::string name = "";
            std::vector<int> v_word;
            uint32_t *str_start = memory + (*a0 / 4);
            bool read = true;
            int start = *a0 % 4;
            while (read)
            {
                std::string word = mips_simulator::dec2bin_u(std::to_string(*str_start), 32);
                for (int i = 8 * (3 - start); i >= 0; i = i - 8)
                {
                    std::string sin_word = word.substr(i, 8);
                    if (sin_word != "00000000")
                    {
                        v_word.push_back(mips_simulator::bin2dec_i(sin_word));
                    }
                    else
                    {
                        read = false;
                        break;
                    }
                }
                if (!read)
                {
                    break;
                }
                start = 0;
                str_start++;
            }

            for (std::vector<int>::iterator iter = v_word.begin(); iter != v_word.end(); iter++)
            {
                char c = *iter;
                name += c;
            }

            int size_t = open(&name[0], a, *a2);
            *v0 = size_t;
            break;
        }
        case 14:
        { // read
            uint32_t *buf = memory + *a1 / 4;
            int size_t = read(*a0, buf, *a2);
            *v0 = mips_simulator::change2uint_without_reverse(mips_simulator::dec2bin(std::to_string(size_t), 32));
            break;
        }
        case 15:
        { // write
            uint32_t *buf = memory + *a1 / 4;

            #ifdef DEBUG
            std::cout << "*a0 = " << *a0 << "; *buf = " << *buf << "; *a2 = " << a2 << std::endl;
            #endif

            int size_t = write(*a0, buf, *a2);
            *v0 = mips_simulator::change2uint_without_reverse(mips_simulator::dec2bin(std::to_string(size_t), 32));
            break;
        }
        case 16:
        { // close
            close(*a0);
            break;
        }
        case 17:
        {
            exit(*a0);
            break;
        }
        default:;
        }
    }

    // i_tsi
    void AddiF(uint32_t *rt, uint32_t *rs, std::string imm)
    {
        // std::cout << "entering addi !!" << std::endl;

        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        long long ll_rs = (long long)i_rs;
        long long i_imm = mips_simulator::bin2dec_i(imm);
        long long ll_res = ll_rs + i_imm;
        if (ll_res > 2147483647 || ll_res < -2147483648)
        {
            std::cout << "overflow" << std::endl;
            exit(1);
        }
        else
        {
            int i_rt = (int)ll_res;
            *rt = mips_simulator::change2uint_without_reverse(mips_simulator::dec2bin(std::to_string(i_rt), 32));
        }
    }

    void AddiuF(uint32_t *rt, uint32_t *rs, std::string imm)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_imm = mips_simulator::bin2dec_i(imm);
        int i_rt = i_rs + i_imm;
        *rt = mips_simulator::change2uint_without_reverse(mips_simulator::dec2bin(std::to_string(i_rt), 32));
    }

    void AndiF(uint32_t *rt, uint32_t *rs, std::string imm)
    {
        for (int i = 0; i < 16; i++)
        {
            imm = '0' + imm;
        }
        std::string s_rs = mips_simulator::dec2bin_u(std::to_string(*rs), 32);
        std::string s_rt = "00000000000000000000000000000000";
        for (int i = 0; i < 32; i++)
        {
            if (s_rs[i] == '1' && imm[i] == '1')
            {
                s_rt[i] = '1';
            }
        }
        *rt = mips_simulator::bin2dec_u(s_rt);
    }

    void OriF(uint32_t *rt, uint32_t *rs, std::string imm)
    {
        for (int i = 0; i < 16; i++)
        {
            imm = '0' + imm;
        }
        std::string s_rs = mips_simulator::dec2bin_u(std::to_string(*rs), 32);
        std::string s_rt = "00000000000000000000000000000000";
        for (int i = 0; i < 32; i++)
        {
            if (s_rs[i] == '1' || imm[i] == '1')
            {
                s_rt[i] = '1';
            }
        }
        *rt = mips_simulator::bin2dec_u(s_rt);
    }

    void XoriF(uint32_t *rt, uint32_t *rs, std::string imm)
    {
        for (int i = 0; i < 16; i++)
        {
            imm = '0' + imm;
        }
        std::string s_rs = mips_simulator::dec2bin_u(std::to_string(*rs), 32);
        std::string s_rt = "00000000000000000000000000000000";
        for (int i = 0; i < 32; i++)
        {
            if (s_rs[i] != imm[i])
            {
                s_rt[i] = '1';
            }
        }
        *rt = mips_simulator::bin2dec_u(s_rt);
    }

    void SltiF(uint32_t *rt, uint32_t *rs, std::string imm)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_imm = mips_simulator::bin2dec_i(imm);
        if (i_rs < i_imm)
        {
            *rt = 1;
        }
        else
        {
            *rt = 0;
        }
    }

    void SltiuF(uint32_t *rt, uint32_t *rs, std::string imm)
    {
        uint32_t ui_imm = mips_simulator::bin2dec_u(imm);
        if (*rs < ui_imm)
        {
            *rt = 1;
        }
        else
        {
            *rt = 0;
        }
    }

    void LuiF(uint32_t *rt, std::string imm)
    {
        for (int i = 0; i < 16; i++)
        {
            imm = imm + '0';
        }
        *rt = mips_simulator::change2uint_without_reverse(imm);
    }

    // i_stl
    int BeqF(uint32_t *rs, uint32_t *rt, uint32_t *pc, std::string imm)
    {
        if (*rs == *rt)
        {
            uint32_t beq = *pc;
            imm = imm + "00";
            int i_imm = mips_simulator::bin2dec_i(imm);
            *pc += i_imm + 4;
            uint32_t cur = *pc;
            return cur - beq;
        }
        return 4;
    }

    int BneF(uint32_t *rs, uint32_t *rt, uint32_t *pc, std::string imm)
    {
        if (*rs != *rt)
        {
            uint32_t bne = *pc;
            imm = imm + "00";
            int i_imm = mips_simulator::bin2dec_i(imm);
            *pc += i_imm + 4;
            uint32_t cur = *pc;
            return cur - bne;
        }
        return 4;
    }

    // i_sl
    int BgezF(uint32_t *rs, uint32_t *pc, std::string imm, uint32_t *ra)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        if (i_rs >= 0)
        {
            uint32_t bgez = *pc;
            imm = imm + "00";
            int i_imm = mips_simulator::bin2dec_i(imm);
            *pc += i_imm + 4;
            uint32_t cur = *pc;
            return cur - bgez;
        }
        return 4;
    }

    int BgezalF(uint32_t *rs, uint32_t *pc, std::string imm, uint32_t *ra)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        *ra = *pc + 4;
        if (i_rs >= 0)
        {
            uint32_t bgezal = *pc;
            imm = imm + "00";
            int i_imm = mips_simulator::bin2dec_i(imm);
            *pc += i_imm + 4;
            uint32_t cur = *pc;
            return cur - bgezal;
        }
        return 4;
    }

    int BgtzF(uint32_t *rs, uint32_t *pc, std::string imm, uint32_t *ra)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        if (i_rs > 0)
        {
            uint32_t bgtz = *pc;
            imm = imm + "00";
            int i_imm = mips_simulator::bin2dec_i(imm);
            *pc += i_imm + 4;
            uint32_t cur = *pc;
            return cur - bgtz;
        }
        return 4;
    }

    int BlezF(uint32_t *rs, uint32_t *pc, std::string imm, uint32_t *ra)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        //    cout << i_rs << "i_rs" << endl;
        if (i_rs <= 0)
        {
            uint32_t blez = *pc;
            imm = imm + "00";
            int i_imm = mips_simulator::bin2dec_i(imm);
            *pc += i_imm + 4;
            uint32_t cur = *pc;
            return cur - blez;
        }
        return 4;
    }

    int BltzalF(uint32_t *rs, uint32_t *pc, std::string imm, uint32_t *ra)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        *ra = *pc + 4;
        if (i_rs < 0)
        {
            uint32_t bltzal = *pc;
            imm = imm + "00";
            int i_imm = mips_simulator::bin2dec_i(imm);
            *pc += i_imm + 4;
            uint32_t cur = *pc;
            return cur - bltzal;
        }
        return 4;
    }

    int BltzF(uint32_t *rs, uint32_t *pc, std::string imm, uint32_t *ra)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        if (i_rs < 0)
        {
            uint32_t bltz = *pc;
            imm = imm + "00";
            int i_imm = mips_simulator::bin2dec_i(imm);
            *pc += i_imm + 4;
            uint32_t cur = *pc;
            return cur - bltz;
        }
        return 4;
    }

    // i_si
    void TeqiF(uint32_t *rs, std::string imm)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_imm = mips_simulator::bin2dec_i(imm);
        if (i_rs == i_imm)
        {
            exit(1);
        }
    }

    void TneiF(uint32_t *rs, std::string imm)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_imm = mips_simulator::bin2dec_i(imm);
        if (i_rs != i_imm)
        {
            exit(1);
        }
    }

    void TgeiF(uint32_t *rs, std::string imm)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_imm = mips_simulator::bin2dec_i(imm);
        if (i_rs >= i_imm)
        {
            exit(1);
        }
    }

    void TgeiuF(uint32_t *rs, std::string imm)
    {
        uint32_t ui_imm = mips_simulator::bin2dec_u(imm);
        if (*rs >= ui_imm)
        {
            exit(1);
        }
    }

    void TltiF(uint32_t *rs, std::string imm)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        int i_imm = mips_simulator::bin2dec_i(imm);
        if (i_rs < i_imm)
        {
            exit(1);
        }
    }

    // rs signed or unsigned? ***
    void TltiuF(uint32_t *rs, std::string imm)
    {
        uint32_t ui_imm = mips_simulator::bin2dec_u(imm);
        if (*rs < ui_imm)
        {
            exit(1);
        }
    }

    // i_ta
    void LbF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        int num_of_byte = addr % 4;
        memory = memory + num_of_word;

        uint32_t ui_cur_word = *memory;
        std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);
        std::string byte = s_cur_word.substr((3 - num_of_byte) * 8, 8);
        //    string byte = s_cur_word.substr(num_of_byte*8,8);
        char sign_byte = byte[0];
        for (int i = 0; i < 24; i++)
        {
            byte = sign_byte + byte;
        }
        uint32_t u_byte = mips_simulator::bin2dec_u(byte);
        *rt = u_byte;
    }

    void LbuF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        int num_of_byte = addr % 4;
        memory = memory + num_of_word;
        uint32_t ui_cur_word = *memory;
        std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);
        //    string byte = s_cur_word.substr(num_of_byte*8,8);
        std::string byte = s_cur_word.substr((3 - num_of_byte) * 8, 8);
        uint32_t u_byte = mips_simulator::bin2dec_u(byte);
        *rt = u_byte;
    }

    void LhF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        int num_of_byte = addr % 4;
        memory = memory + num_of_word;

        uint32_t ui_cur_word = *memory;
        std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);
        std::string half = s_cur_word.substr((2 - num_of_byte) * 8, 16);
        char sign_half = half[0];
        for (int i = 0; i < 16; i++)
        {
            half = sign_half + half;
        }
        uint32_t u_byte = mips_simulator::bin2dec_u(half);
        *rt = u_byte;
    }

    void LhuF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        int num_of_byte = addr % 4;
        memory = memory + num_of_word;
        uint32_t ui_cur_word = *memory;
        std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);
        std::string half = s_cur_word.substr((2 - num_of_byte) * 8, 16);
        uint32_t u_byte = mips_simulator::bin2dec_u(half);
        *rt = u_byte;
    }

    void LwF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        memory = memory + num_of_word;
        uint32_t ui_cur_word = *memory;
        std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);
        //    s_cur_word = change_order_32(s_cur_word);
        ui_cur_word = mips_simulator::bin2dec_u(s_cur_word);
        *rt = ui_cur_word;
    }

    void LwlF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        int num_of_byte = addr % 4;
        memory = memory + num_of_word;
        uint32_t ui_cur_word = *memory;
        std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);

        std::string bytes = s_cur_word.substr(8 * (3 - num_of_byte));
        //    if (num_of_byte == 1) {
        //        bytes = change_order_16(bytes);
        //    } else if (num_of_byte == 2) {
        //        bytes = change_order_24(bytes);
        //    } else if (num_of_byte == 3) {
        //        bytes = change_order_32(bytes);
        //    }
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        for (int i = 0; i < (num_of_byte + 1) * 8; i++)
        {
            s_rt[i] = bytes[i];
        }
        *rt = mips_simulator::bin2dec_u(s_rt);
    }

    void LwrF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        int num_of_byte = addr % 4;
        memory = memory + num_of_word;
        uint32_t ui_cur_word = *memory;
        std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);
        std::string bytes = s_cur_word.substr(0, 32 - (num_of_byte)*8);

        //    if (num_of_byte == 0) {
        //        bytes = change_order_32(bytes);
        //    } else if (num_of_byte == 1) {
        //        bytes = change_order_24(bytes);
        //    } else if (num_of_byte == 2) {
        //        bytes = change_order_16(bytes);
        //    }

        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        for (int i = 0; i < 32 - 8 * num_of_byte; i++)
        {
            s_rt[i + 8 * num_of_byte] = bytes[i];
        }
        *rt = mips_simulator::bin2dec_u(s_rt);
    }

    void LlF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        memory = memory + num_of_word;
        uint32_t ui_cur_word = *memory;
        std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);
        //    s_cur_word = change_order_32(s_cur_word);
        ui_cur_word = mips_simulator::bin2dec_u(s_cur_word);
        *rt = ui_cur_word;
    }

    void SbF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        int num_of_byte = addr % 4;
        memory = memory + num_of_word;
        uint32_t ui_cur_word = *memory;
        std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        std::string s_rtlow = s_rt.substr(24, 8);
        for (int i = 0; i < 8; i++)
        {
            s_cur_word[(3 - num_of_byte) * 8 + i] = s_rtlow[i];
        }
        uint32_t s_aft_word = mips_simulator::bin2dec_u(s_cur_word);
        *memory = s_aft_word;
    }

    void ShF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        int num_of_byte = addr % 4;
        memory = memory + num_of_word;
        uint32_t ui_cur_word = *memory;
        std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        std::string s_rtlow = s_rt.substr(16, 16);
        for (int i = 0; i < 16; i++)
        {
            s_cur_word[(2 - num_of_byte) * 8 + i] = s_rtlow[i];
        }
        uint32_t s_aft_word = mips_simulator::bin2dec_u(s_cur_word);
        *memory = s_aft_word;
    }

    void SwF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        memory = memory + num_of_word;
        //    cout << "*rt" << *rt << endl;
        //    string s_rt = change_order_32(dec2bin_u(to_string(*rt),32));
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        //    cout << s_rt << "s_rt" << endl;
        uint32_t s_aft_word = mips_simulator::bin2dec_u(s_rt);
        *memory = s_aft_word;
    }

    void SwlF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        int num_of_byte = addr % 4;
        memory = memory + num_of_word;
        uint32_t ui_cur_word = *memory;
        std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        std::string bytes = s_rt.substr(0, (num_of_byte + 1) * 8);
        //    if (num_of_byte == 1) {
        //        bytes = change_order_16(bytes);
        //    } else if (num_of_byte == 2) {
        //        bytes = change_order_24(bytes);
        //    } else if (num_of_byte == 3) {
        //        bytes = change_order_32(bytes);
        //    }

        for (int i = 0; i < (num_of_byte + 1) * 8; i++)
        {
            s_cur_word[8 * (3 - num_of_byte) + i] = bytes[i];
        }
        *memory = mips_simulator::bin2dec_u(s_cur_word);
    }

    void SwrF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        int num_of_byte = addr % 4;
        memory = memory + num_of_word;
        uint32_t ui_cur_word = *memory;
        std::string s_cur_word = mips_simulator::dec2bin_u(std::to_string(ui_cur_word), 32);
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);

        std::string bytes = s_rt.substr(8 * (num_of_byte));
        //    if (num_of_byte == 0) {
        //        bytes = change_order_32(bytes);
        //    } else if (num_of_byte == 1) {
        //        bytes = change_order_24(bytes);
        //    } else if (num_of_byte == 2) {
        //        bytes = change_order_16(bytes);
        //    }

        for (int i = 0; i < (4 - num_of_byte) * 8; i++)
        {
            s_cur_word[i] = bytes[i];
        }
        *memory = mips_simulator::bin2dec_u(s_cur_word);
    }

    void ScF(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory)
    {
        int i_rs = mips_simulator::change2int(std::to_string(*rs));
        char sign = imm[0];
        for (int i = 0; i < 16; i++)
        {
            imm = sign + imm;
        }
        int offset = mips_simulator::bin2dec_i(imm);
        int addr = i_rs + offset;
        int num_of_word = addr / 4;
        memory = memory + num_of_word;
        //    cout << "*rt" << *rt << endl;
        //    string s_rt = change_order_32(dec2bin_u(to_string(*rt),32));
        std::string s_rt = mips_simulator::dec2bin_u(std::to_string(*rt), 32);
        //    cout << s_rt << "s_rt" << endl;
        uint32_t s_aft_word = mips_simulator::bin2dec_u(s_rt);
        *memory = s_aft_word;
    }

    int JF(uint32_t *pc, std::string addr)
    {
        uint32_t j = *pc;
        addr = addr + "00";
        std::string s_pc = mips_simulator::dec2bin_u(std::to_string(*pc), 32);
        for (int i = 0; i < 28; i++)
        {
            s_pc[i + 4] = addr[i];
        }
        *pc = mips_simulator::bin2dec_u(s_pc);
        uint32_t cur = *pc;
        return cur - j;
    }

    int JalF(uint32_t *pc, std::string addr, uint32_t *ra)
    {
        uint32_t jal = *pc;
        *ra = *pc + 4;
        addr = addr + "00";
        std::string s_pc = mips_simulator::dec2bin_u(std::to_string(*pc), 32);
        for (int i = 0; i < 28; i++)
        {
            s_pc[i + 4] = addr[i];
        }
        *pc = mips_simulator::bin2dec_u(s_pc);
        uint32_t cur = *pc;
        return cur - jal;
    }

} // namespace mips_funct

typedef void R_dst_Type(uint32_t *rd, uint32_t *rs, uint32_t *rt);
typedef void R_ds_Type(uint32_t *rd, uint32_t *rs);
typedef void R_sthl_Type(uint32_t *rs, uint32_t *rt, uint32_t *hi, uint32_t *lo); // hi lo
typedef void R_stt_Type(uint32_t *rs, uint32_t *rt);
typedef void R_dtshamt_Type(uint32_t *rd, uint32_t *rt, std::string shamt);
typedef void R_dts_Type(uint32_t *rd, uint32_t *rt, uint32_t *rs);
typedef void I_tsi_Type(uint32_t *rt, uint32_t *rs, std::string imm);
typedef int I_stl_Type(uint32_t *rs, uint32_t *rt, uint32_t *pc, std::string imm);
typedef int I_sl_Type(uint32_t *rs, uint32_t *pc, std::string imm, uint32_t *ra);
typedef void I_si_Type(uint32_t *rs, std::string imm);
typedef void I_ta_Type(uint32_t *rs, uint32_t *rt, std::string imm, uint32_t *memory);

// the order of int op_type
R_dst_Type *R_dst_functions[] = {mips_funct::AddF, mips_funct::AdduF, mips_funct::AndF, mips_funct::MulF, mips_funct::NorF, mips_funct::OrF, mips_funct::SubF, mips_funct::SubuF, mips_funct::XorF, mips_funct::SltF, mips_funct::SltuF};
R_ds_Type *R_ds_function[] = {mips_funct::CloF, mips_funct::ClzF};
R_sthl_Type *R_sthl_function[] = {mips_funct::DivF, mips_funct::DivuF, mips_funct::MultF, mips_funct::MultuF, mips_funct::MaddF, mips_funct::MadduF, mips_funct::MsubF, mips_funct::MsubuF};
R_stt_Type *R_stt_function[] = {mips_funct::TeqF, mips_funct::TneF, mips_funct::TgeF, mips_funct::TgeuF, mips_funct::TltF, mips_funct::TltuF};
R_dtshamt_Type *R_dtshamt_function[] = {mips_funct::SllF, mips_funct::SraF, mips_funct::SrlF};
R_dts_Type *R_dts_function[] = {mips_funct::SllvF, mips_funct::SravF, mips_funct::SrlvF};
I_tsi_Type *I_tsi_function[] = {mips_funct::AddiF, mips_funct::AddiuF, mips_funct::AndiF, mips_funct::OriF, mips_funct::XoriF, mips_funct::SltiF, mips_funct::SltiuF};
I_stl_Type *I_stl_function[] = {mips_funct::BeqF, mips_funct::BneF};
I_sl_Type *I_sl_function[] = {mips_funct::BgezF, mips_funct::BgezalF, mips_funct::BgtzF, mips_funct::BlezF, mips_funct::BltzalF, mips_funct::BltzF};
I_si_Type *I_si_function[] = {mips_funct::TeqiF, mips_funct::TneiF, mips_funct::TgeiF, mips_funct::TgeiuF, mips_funct::TltiF, mips_funct::TltiuF};
I_ta_Type *I_ta_function[] = {mips_funct::LbF, mips_funct::LbuF, mips_funct::LhF, mips_funct::LhuF, mips_funct::LwF, mips_funct::LwlF, mips_funct::LwrF, mips_funct::LlF, mips_funct::SbF, mips_funct::ShF, mips_funct::SwF, mips_funct::SwlF, mips_funct::SwrF, mips_funct::ScF};

#endif
