#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include "utils.h"
#include "dic.h"
#include "scanner.h"

// Preprocessing
void Scanner::preprocess(std::vector<std::string> &vec_input_raw)
{
    bool find_data = false;
    bool find_text = false;
    int pc = 0x400000;
    std::string line_processed = "";

    // scan first
    for (std::vector<std::string>::iterator iter_line = vec_input_raw.begin(); iter_line != vec_input_raw.end(); ++iter_line)
    {
        std::string s_line = *iter_line;

        int len_line = s_line.length();
        int find_q = s_line.find("\"");

        // replace "," with " "; the "," is not in strings or labels
        for (int i = 0; i < len_line; i++)
        {
            if (s_line[i] == ',' && (i < find_q || find_q == -1))
            {
                s_line[i] = ' ';
            }
        }

        // remove comment
        std::string::size_type pos_cmt = s_line.find('#');
        if (pos_cmt != s_line.npos)
        {
            s_line = s_line.substr(0, pos_cmt);
        }

        std::vector<std::string> vec_line_word = mips_simulator::tokenizer(s_line);

        // mark .data, .text, syscall, labels, and push static data and instructions
        if (vec_line_word.size() == 1)
        { // len == 1 or syscall
            if (vec_line_word.front() == ".data")
            { // .data
                find_data = true;
            }
            else if (vec_line_word.front() == ".text")
            { //.text
                find_text = true;
            }
            else if (vec_line_word.front() == "syscall")
            { // syscall
                vec_text.push_back(vec_line_word.front());
                pc = pc + 4;
            }
            else
            { // label: instruction
                label.insert(std::pair<std::string, int>(vec_line_word.front(), pc));
            }
        }
        else if (vec_line_word.size() == 0)
        { // len == 0 empty line
        }
        else
        { // instruction or static data
            if (find_data == true && find_text == false)
            { // static data
                for (std::vector<std::string>::iterator iter = vec_line_word.begin() + 1; iter != vec_line_word.end(); iter++)
                {
                    line_processed += *iter;
                    line_processed += " ";
                }
                vec_data.push_back(line_processed);
            }
            else if (find_text == true)
            { // instruction
                std::vector<std::string>::iterator first_ele = vec_line_word.begin();
                std::map<std::string, std::string>::iterator first_r = opcode_dic::op_r_dic.find(*first_ele);
                std::map<std::string, std::string>::iterator first_i = opcode_dic::op_i_dic.find(*first_ele);
                std::map<std::string, std::string>::iterator first_j = opcode_dic::op_j_dic.find(*first_ele);

                if (first_r == opcode_dic::op_r_dic.end() && first_i == opcode_dic::op_i_dic.end() && first_j == opcode_dic::op_j_dic.end())
                { // instruction with label:

                    // mark the position of the label
                    std::string fe = *first_ele;
                    int pos_colon = fe.find(":");
                    fe[pos_colon] = ' ';

                    std::string l;
                    std::string p;

                    std::vector<std::string> label_op = mips_simulator::tokenizer(fe);

                    l = label_op.at(0);
                    l = l + ':';
                    label.insert(std::pair<std::string, int>(l, pc));

                    // push the rest into the vector
                    std::vector<std::string>::iterator iter = vec_line_word.begin();
                    iter++;
                    for (std::vector<std::string>::iterator iter = vec_line_word.begin() + 1; iter != vec_line_word.end(); iter++)
                    {
                        line_processed += *iter;
                        line_processed += " ";
                    }

                    if (label_op.size() == 2)
                    {
                        p = label_op.at(1);
                        line_processed = p + " " + line_processed;
                    }

                    vec_text.push_back(line_processed);
                }
                else
                { // instruction without label
                    for (std::vector<std::string>::iterator iter = vec_line_word.begin(); iter != vec_line_word.end(); iter++)
                    {
                        line_processed += *iter;
                        line_processed += " ";
                    }
                    vec_text.push_back(line_processed);
                }
                pc = pc + 4;
            }
        }
        vec_line_word.clear();
        line_processed = "";
    }
}

// Tranlate a line of mips code to corresponding machine code
Instruction Scanner::mips2bin(std::string line, int pc)
{

    std::map<std::string, std::string>::iterator iter_map;
    std::map<std::string, std::string>::iterator iter_rgt;
    std::map<std::string, int>::iterator iter_label;
    // std::map<std::string, int>::iterator iter_b_instruction;

    std::string shamt;
    std::string label_quo;
    std::string s_offset;
    std::string imm = "";
    std::string rs = "";

    std::vector<std::string> instr_tokens = mips_simulator::tokenizer(line);

    std::vector<std::string>::iterator iter_vec = instr_tokens.begin();
    std::vector<std::string>::iterator iter = instr_tokens.begin();

    int instruct_type = detect_instruction_type(iter_vec);

    int code_type_r = 0;
    int code_type_i = 0;
    int pos_l_b; // left bracket
    int pos_r_b;
    int pos_label = 0;
    int i_offset = 0;
    Instruction instruction;

    switch (instruct_type)
    {
    case 1:
    { // r

        code_type_r = determine_rgt_seq_r(*iter_vec);

        // assign values of the machine code to Instruction
        switch (code_type_r)
        {
        case 1:
        { // dst
            iter_map = opcode_dic::r_dst.find(*iter);
            instruction.op = iter_map->second;
            iter_map = r_funct_dic::r_funct.find(*iter);
            instruction.funct = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rd = iter_rgt->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rs = iter_rgt->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rt = iter_rgt->second;
            instruction.instruction_type = 11;
            break;
        }

        case 2:
        { // ds
            iter_map = opcode_dic::r_ds.find(*iter);
            instruction.op = iter_map->second;
            iter_map = r_funct_dic::r_funct.find(*iter);
            instruction.funct = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rd = iter_rgt->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rs = iter_rgt->second;
            instruction.instruction_type = 12;
            break;
        }

        case 3:
        { // st
            iter_map = opcode_dic::r_st.find(*iter);
            instruction.op = iter_map->second;
            iter_map = r_funct_dic::r_funct.find(*iter);
            instruction.funct = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rs = iter_rgt->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rt = iter_rgt->second;
            instruction.instruction_type = 13;
            break;
        }

        case 4:
        { // dtshamt
            iter_map = opcode_dic::r_dtshamt.find(*iter);
            instruction.op = iter_map->second;
            iter_map = r_funct_dic::r_funct.find(*iter);
            instruction.funct = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rd = iter_rgt->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rt = iter_rgt->second;
            iter++;
            shamt = mips_simulator::dec2bin(*iter, 5);
            instruction.shamt = shamt;
            instruction.instruction_type = 14;
            break;
        }

        case 5:
        { // sd // default to 31?
            iter_map = opcode_dic::r_sd.find(*iter);
            instruction.op = iter_map->second;
            iter_map = r_funct_dic::r_funct.find(*iter);
            instruction.funct = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rs = iter_rgt->second;
            iter++;
            if (iter != instr_tokens.end())
            {
                iter_rgt = rgt_dic::rgt_dic.find(*iter);
                instruction.rd = iter_rgt->second;
            }
            else
            { // default is rgt 31
                instruction.rd = "11111";
            }
            instruction.instruction_type = 15;
            break;
        }

        case 6:
        { // s
            iter_map = opcode_dic::r_s.find(*iter);
            instruction.op = iter_map->second;
            iter_map = r_funct_dic::r_funct.find(*iter);
            instruction.funct = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rs = iter_rgt->second;
            instruction.instruction_type = 16;
            break;
        }

        case 7:
        { // d
            iter_map = opcode_dic::r_d.find(*iter);
            instruction.op = iter_map->second;
            iter_map = r_funct_dic::r_funct.find(*iter);
            instruction.funct = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rd = iter_rgt->second;
            instruction.instruction_type = 17;
            break;
        }

        case 8:
        { // dts
            iter_map = opcode_dic::r_dts.find(*iter);
            instruction.op = iter_map->second;
            iter_map = r_funct_dic::r_funct.find(*iter);
            instruction.funct = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rd = iter_rgt->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rt = iter_rgt->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rs = iter_rgt->second;
            instruction.instruction_type = 18;
            break;
        }

        case 9:
        { // syscall
            iter_map = opcode_dic::r_syscall.find(*iter);
            instruction.op = iter_map->second;
            iter_map = r_funct_dic::r_funct.find(*iter);
            instruction.funct = iter_map->second;
            instruction.instruction_type = 19;
            break;
        }

        default:;
        }
        // FIXME: why need pc+4??
        pc += 4;
        instruction.m_code = instruction.op +
                             instruction.rs +
                             instruction.rt +
                             instruction.rd +
                             instruction.shamt +
                             instruction.funct;
        break;
    }

    case 2:
    { // i
        code_type_i = determine_rgt_seq_i(*iter_vec);

        switch (code_type_i)
        {
        case 1:
        { // tsi
            iter_map = opcode_dic::i_tsi.find(*iter);
            instruction.op = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rt = iter_rgt->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rs = iter_rgt->second;
            iter++;
            instruction.immediate = mips_simulator::dec2bin(*iter, 16);
            instruction.instruction_type = 21;
            pc += 4;
            break;
        }
        case 2: // ti lui ***
            iter_map = opcode_dic::i_ti.find(*iter);
            instruction.op = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rt = iter_rgt->second;
            iter++;
            instruction.immediate = mips_simulator::dec2bin(*iter, 16);
            instruction.instruction_type = 22;
            pc += 4;
            break;
        case 3:
        { // stl both label and offset
            iter_map = opcode_dic::i_stl.find(*iter);
            instruction.op = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rs = iter_rgt->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rt = iter_rgt->second;
            iter++;
            label_quo = *iter + ':';
            iter_label = label.find(label_quo);

            if (iter_label != label.end())
            { // label
                pos_label = iter_label->second;
                i_offset = (pos_label - (pc + 4)) / 4;
                s_offset = std::to_string(i_offset);
                instruction.immediate = mips_simulator::dec2bin(s_offset, 16);
            }
            else
            { // offset
                instruction.immediate = mips_simulator::dec2bin(*iter, 16);
            }
            instruction.instruction_type = 23;
            pc += 4;
            break;
        }
        case 4:
        { // sl both label and offset
            iter_map = opcode_dic::i_sl.find(*iter);
            instruction.op = iter_map->second;
            if (*iter == "bgez")
            {
                instruction.rt = "00001";
            }
            else if (*iter == "bgezal")
            {
                instruction.rt = "10001";
            }
            else if (*iter == "bgtz" || *iter == "blez" || *iter == "bltz")
            {
                instruction.rt = "00000";
            }
            else
            {
                instruction.rt = "10000";
            }
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rs = iter_rgt->second;
            iter++;
            label_quo = *iter + ':';
            iter_label = label.find(label_quo);
            if (iter_label != label.end())
            { // label
                pos_label = iter_label->second;
                i_offset = (pos_label - (pc + 4)) / 4;
                s_offset = std::to_string(i_offset);
                instruction.immediate = mips_simulator::dec2bin(s_offset, 16);
            }
            else
            { // offset
                instruction.immediate = mips_simulator::dec2bin(*iter, 16);
            }
            pc += 4;
            instruction.instruction_type = 24;
            break;
        }
        case 5:
        { // si
            iter_map = opcode_dic::i_si.find(*iter);
            std::string optype = *iter;
            instruction.op = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rs = iter_rgt->second;
            iter++;
            instruction.immediate = mips_simulator::dec2bin(*iter, 16);
            if (optype == "teqi")
            {
                instruction.rt = "01100";
            }
            else if (optype == "tnei")
            {
                instruction.rt = "01110";
            }
            else if (optype == "tgei")
            {
                instruction.rt = "01000";
            }
            else if (optype == "tgeiu")
            {
                instruction.rt = "01001";
            }
            else if (optype == "tlti")
            {
                instruction.rt = "01010";
            }
            else if (optype == "tltiu")
            {
                instruction.rt = "01011";
            }
            pc += 4;
            instruction.instruction_type = 25;
            break;
        }
        case 6:
        { // ta
            iter_map = opcode_dic::i_ta.find(*iter);
            instruction.op = iter_map->second;
            iter++;
            iter_rgt = rgt_dic::rgt_dic.find(*iter);
            instruction.rt = iter_rgt->second;
            iter++;
            s_offset = *iter;
            pos_l_b = s_offset.find('(');
            pos_r_b = s_offset.find(')');
            for (int i = 0; i < pos_l_b; i++)
            {
                imm += s_offset[i];
            }
            instruction.immediate = mips_simulator::dec2bin(imm, 16);
            for (int i = pos_l_b + 1; i < pos_r_b; i++)
            {
                rs += s_offset[i];
            }
            iter_rgt = rgt_dic::rgt_dic.find(rs);
            instruction.rs = iter_rgt->second;
            pc += 4;
            instruction.instruction_type = 26;
            break;
        }
        default:;
        }
        pc += 4;
        instruction.m_code = instruction.op +
                             instruction.rs +
                             instruction.rt +
                             instruction.immediate;
        break;
    }

    case 3:
    { // j both label and offset
        iter_map = opcode_dic::op_j_dic.find(*iter);
        instruction.op = iter_map->second;
        iter++;
        label_quo = *iter + ':';
        iter_label = label.find(label_quo);
        if (iter_label != label.end())
        { // label
            pos_label = iter_label->second;
            std::string s_pos_label = mips_simulator::dec2bin(std::to_string(pos_label), 32);
            s_offset = s_pos_label.substr(4, 26);
            instruction.immediate = s_offset;
        }
        else
        { // offset
            /*??? hex*/
            instruction.immediate = mips_simulator::dec2bin(*iter, 26);
        }
        pc += 4;
        instruction.m_code = instruction.op + instruction.immediate;
        break;
    }

    default:;
    }

    return instruction;
}

// Tranlate a line of static data to corresponding binary bode, which is to be stored in the static data section
// return 0/1/2/3 num of used mem
int Scanner::static_data2bin(std::vector<std::string> *vec_whole_data, std::string line, int pos, int *nob)
{
    std::vector<std::string> vec_original_word;
    std::vector<std::string> vec_bin_sin_word;

    std::string word;
    std::istringstream is(line);
    while (is >> word)
    {
        vec_original_word.push_back(word);
    }
    is.clear();

    std::vector<std::string>::iterator iter = vec_original_word.begin();

    // 2. type
    if (*iter == ".ascii")
    { //.ascii str
        int q_l = line.find("\"");
        int q_r = line.find_last_of("\"");
        std::string str_q = line.substr(q_l);
        std::string str = line.substr(q_l + 1, q_r - q_l - 1);
        int len = str.length();
        std::string sin_char; // four character

        for (int i = 0; i < len; i++)
        {
            if (str[i] == '\\')
            {
                ++i;
                vec_bin_sin_word.push_back(escape_word_dic::escape_word_dic.find(str[i])->second);
            }
            else
            {
                sin_char = mips_simulator::dec2bin_u(std::to_string(str[i]), 8);
                vec_bin_sin_word.push_back(sin_char);
            }
        }

        int num_of_row = vec_bin_sin_word.size() / 4;
        int num_of_rest = vec_bin_sin_word.size() % 4;
        std::string com_char;
        std::vector<std::string>::iterator iter_vbsw = vec_bin_sin_word.begin();

        std::string last;

        // concatenate every four chars in little endian order
        for (int i = 0; i < num_of_row; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                com_char = *iter_vbsw + com_char; // lleh
                iter_vbsw++;
            }
            vec_whole_data->push_back(com_char);
            com_char = "";
            *nob += 4;
        }

        // concatenate the rest chars in little endian order
        for (int i = 0; i < num_of_rest; i++)
        {
            com_char = *iter_vbsw + com_char;
            iter_vbsw++;
        }

        // using 0 to fill up rest of the places, to align into 32 bits
        if (num_of_rest != 0)
        {
            int num_of_0 = (4 - num_of_rest) * 8;
            for (int i = 0; i < num_of_0; i++)
            {
                com_char = '0' + com_char;
            }
            vec_whole_data->push_back(com_char);
            *nob += 4;
        }

        pos = num_of_rest;
    }
    else if (*iter == ".asciiz")
    { // .asciiz str
        int q_l = line.find("\"");
        int q_r = line.find_last_of("\"");
        std::string str_q = line.substr(q_l);
        std::string str = line.substr(q_l + 1, q_r - q_l - 1);
        int len = str.length();
        std::string com_char;
        std::string sin_char; // four character

        for (int i = 0; i < len; i++)
        {
            if (str[i] == '\\')
            {
                i++;
                vec_bin_sin_word.push_back(escape_word_dic::escape_word_dic.find(str[i])->second);
            }
            else
            {
                sin_char = mips_simulator::dec2bin_u(std::to_string(str[i]), 8);
                vec_bin_sin_word.push_back(sin_char);
            }
        }
        vec_bin_sin_word.push_back("00000000");
        int num_of_row = vec_bin_sin_word.size() / 4;
        int num_of_rest = vec_bin_sin_word.size() % 4;

        std::vector<std::string>::iterator iter_vbsw = vec_bin_sin_word.begin();

        // concatenate every four chars in little endian order
        for (int i = 0; i < num_of_row; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                com_char = *iter_vbsw + com_char;
                iter_vbsw++;
            }
            vec_whole_data->push_back(com_char);
            com_char = "";
            *nob += 4;
        }

        // concatenate the rest chars in little endian order
        for (int i = 0; i < num_of_rest; i++)
        {
            com_char = *iter_vbsw + com_char;
            iter_vbsw++;
        }

        // using 0 to fill up rest of the places, to align into 32 bits
        if (num_of_rest != 0)
        {
            int num_of_0 = (4 - num_of_rest) * 8;
            for (int i = 0; i < num_of_0; i++)
            {
                com_char = '0' + com_char;
            }
            vec_whole_data->push_back(com_char);
            *nob += 4;
        }
        pos = num_of_rest;
    }
    else if (*iter == ".word")
    { // little endian
        iter++;
        std::string sin_word;

        for (std::vector<std::string>::iterator iter_num = vec_original_word.begin() + 1; iter_num != vec_original_word.end(); iter_num++)
        {
            sin_word = mips_simulator::dec2bin(*iter_num, 32);
            vec_whole_data->push_back(sin_word);
            *nob += 4;
        }
        pos = 0;
    }
    else if (*iter == ".byte")
    {
        iter++;
        std::string sin_byte;
        std::vector<std::string> vec_sin_byte;

        for (std::vector<std::string>::iterator iter_num = vec_original_word.begin() + 1; iter_num != vec_original_word.end(); iter_num++)
        {
            sin_byte = mips_simulator::dec2bin(*iter_num, 8);
            vec_sin_byte.push_back(sin_byte);
        }

        int len = vec_sin_byte.size();
        int num_of_row = len / 4;
        int num_of_rest = len % 4;
        std::string com_byte;
        std::vector<std::string>::iterator iter_vsb = vec_sin_byte.begin();

        for (int i = 0; i < num_of_row; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                com_byte = *iter_vsb + com_byte;
                iter_vsb++;
            }
            vec_whole_data->push_back(com_byte);
            com_byte = "";
            *nob += 4;
        }

        for (int i = 0; i < num_of_rest; i++)
        {
            com_byte = *iter_vsb + com_byte;
            iter_vsb++;
        }

        if (num_of_rest != 0)
        {
            int num_of_0 = (4 - num_of_rest) * 8;
            for (int i = 0; i < num_of_0; i++)
            {
                com_byte = '0' + com_byte;
            }
            vec_whole_data->push_back(com_byte);
            *nob += 4;
        }
        pos = num_of_rest;
    }
    else if (*iter == ".half")
    { // half; little endian
        iter++;
        std::string sin_half;
        std::vector<std::string> vec_sin_half;

        for (std::vector<std::string>::iterator iter_num = vec_original_word.begin() + 1; iter_num != vec_original_word.end(); iter_num++)
        {
            sin_half = mips_simulator::dec2bin(*iter_num, 16);
            vec_sin_half.push_back(sin_half);
        }

        int len = vec_sin_half.size();
        int num_of_row = len / 2;
        int num_of_rest = len % 2;
        std::string com_half;

        std::vector<std::string>::iterator iter_vsh = vec_sin_half.begin();

        for (int i = 0; i < num_of_row; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                com_half = *iter_vsh + com_half;
                iter_vsh++;
            }
            vec_whole_data->push_back(com_half);
            com_half = "";
            *nob += 4;
        }

        for (int i = 0; i < num_of_rest; i++)
        {
            com_half = *iter_vsh + com_half;
            iter_vsh++;
        }

        if (num_of_rest != 0)
        {
            int num_of_0 = 16;
            for (int i = 0; i < num_of_0; i++)
            {
                com_half = '0' + com_half;
            }
            vec_whole_data->push_back(com_half);
            *nob += 4;
        }
        pos = num_of_rest * 2;
    }

    return pos;
}

// To detect whether the instruction is R type, I type, or J type, and return an integer to represent the type
// 1: R type
// 2: I type
// 3: J type
int Scanner::detect_instruction_type(std::vector<std::string>::iterator funct)
{
    int instruction_type = 0;

    std::map<std::string, std::string>::iterator iter_map = opcode_dic::op_r_dic.find(*funct);
    if (iter_map == opcode_dic::op_r_dic.end())
    {
        iter_map = opcode_dic::op_i_dic.find(*funct);
        if (iter_map == opcode_dic::op_i_dic.end())
        {                         // not i
            instruction_type = 3; // is j
        }
        else
        { // is i instruction
            instruction_type = 2;
        }
    }
    else
    { // is r instruction
        instruction_type = 1;
    }

    return instruction_type;
}

// This function determines the sequence of registers of R type instruction, an return an integer represent the sequence
//
int Scanner::determine_rgt_seq_r(std::string opcode)
{
    int code_type_r;

    std::map<std::string, std::string>::iterator iter_map = opcode_dic::r_dst.find(opcode);
    if (iter_map == opcode_dic::r_dst.end())
    {
        iter_map = opcode_dic::r_ds.find(opcode);
        if (iter_map == opcode_dic::r_ds.end())
        {
            iter_map = opcode_dic::r_st.find(opcode);
            if (iter_map == opcode_dic::r_st.end())
            {
                iter_map = opcode_dic::r_dtshamt.find(opcode);
                if (iter_map == opcode_dic::r_dtshamt.end())
                {
                    iter_map = opcode_dic::r_sd.find(opcode);
                    if (iter_map == opcode_dic::r_sd.end())
                    {
                        iter_map = opcode_dic::r_s.find(opcode);
                        if (iter_map == opcode_dic::r_s.end())
                        {
                            iter_map = opcode_dic::r_d.find(opcode);
                            if (iter_map == opcode_dic::r_d.end())
                            {
                                iter_map = opcode_dic::r_dts.find(opcode);
                                if (iter_map == opcode_dic::r_dts.end())
                                { // is syscall
                                    code_type_r = 9;
                                }
                                else
                                { // is dts
                                    code_type_r = 8;
                                }
                            }
                            else
                            { // is d
                                code_type_r = 7;
                            }
                        }
                        else
                        { // is s
                            code_type_r = 6;
                        }
                    }
                    else
                    { // is sd
                        code_type_r = 5;
                    }
                }
                else
                { // is dtshamt
                    code_type_r = 4;
                }
            }
            else
            { // is st
                code_type_r = 3;
            }
        }
        else
        { // is ds
            code_type_r = 2;
        }
    }
    else
    { // is dst
        code_type_r = 1;
    }

    return (code_type_r);
}

// This function determines the sequence of registers of I type instruction
int Scanner::determine_rgt_seq_i(std::string opcode)
{
    int code_type_i;

    std::map<std::string, std::string>::iterator iter_map = opcode_dic::i_tsi.find(opcode);

    if (iter_map == opcode_dic::i_tsi.end())
    {
        iter_map = opcode_dic::i_ti.find(opcode);
        if (iter_map == opcode_dic::i_ti.end())
        {
            iter_map = opcode_dic::i_stl.find(opcode);
            if (iter_map == opcode_dic::i_stl.end())
            {
                iter_map = opcode_dic::i_sl.find(opcode);
                if (iter_map == opcode_dic::i_sl.end())
                {
                    iter_map = opcode_dic::i_si.find(opcode);
                    if (iter_map == opcode_dic::i_si.end())
                    { // is ta
                        code_type_i = 6;
                    }
                    else
                    { // is si
                        code_type_i = 5;
                    }
                }
                else
                { // is sl
                    code_type_i = 4;
                }
            }
            else
            { // is stl
                code_type_i = 3;
            }
        }
        else
        { // is ti
            code_type_i = 2;
        }
    }
    else
    { // is tsi
        code_type_i = 1;
    }

    return (code_type_i);
}

// Store machine codes into main memory
void Scanner::store_machine_code(uint32_t *&pc_ptr, std::vector<std::string> &vec_machine_code)
{
    int pc_l = 0x400000; // used to mark the position of the label
    Instruction instruction;
    // store the machine code
    for (std::vector<std::string>::iterator iter_line = vec_machine_code.begin(); iter_line != vec_machine_code.end(); ++iter_line)
    {
        instruction = mips2bin(*iter_line, pc_l);
        uint32_t ui_mc = mips_simulator::change2uint(instruction.m_code);
        uint32_t *ui_mc_ptr = &ui_mc;
        memcpy(pc_ptr, ui_mc_ptr, 4);
        pc_ptr++;
        pc_l += 4;
    }
}

// Store static data into main memory
int Scanner::store_static_data(uint32_t *&data_ptr, std::vector<std::string> &vec_data)
{
    // store the static data
    int rest = 0;
    int num_of_byte = 0;
    std::vector<std::string> vec_data_bin;

    for (std::vector<std::string>::iterator iter_line = vec_data.begin(); iter_line != vec_data.end(); iter_line++)
    {   
        std::cout << "chars = " << *iter_line << std::endl;
        rest = static_data2bin(&vec_data_bin, *iter_line, rest, &num_of_byte);
    }

    for (std::vector<std::string>::iterator iter = vec_data_bin.begin(); iter != vec_data_bin.end(); iter++)
    {
        std::cout << "data bin = " << *iter << std::endl;

        uint32_t ui_data = mips_simulator::change2uint_without_reverse(*iter);
        uint32_t *ui_data_ptr = &ui_data;
        memcpy(data_ptr, ui_data_ptr, 4);
        data_ptr++;
    }

    return num_of_byte;

    
}
