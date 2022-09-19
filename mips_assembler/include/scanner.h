#ifndef SCANNER_H_
#define SCANNER_H_

#include <vector>
#include <string>
#include <iterator>
#include <map>

struct Instruction
{
    int instruction_type = 0;
    std::string m_code;
    std::string op = "000000";
    std::string rs = "00000";
    std::string rt = "00000";
    std::string rd = "00000";
    std::string shamt = "00000";
    std::string funct = "00000";
    std::string immediate;
    std::string address;
};


// write construction and destruction functions
// FIXME: should change the Scanner to Assembler
class Scanner
{
public:
    // vectors
    std::vector<std::string> vec_data;
    std::vector<std::string> vec_text;

    // label map
    std::map<std::string,int> label {}; // <label:, pc>

    void preprocess(std::vector<std::string> &vec_input_raw);

    // FIXME: store should be changed to load
    void store_machine_code(uint32_t *&pc_ptr, std::vector<std::string> &vec_machine_code);
    int store_static_data(uint32_t *&data_ptr, std::vector<std::string> &vec_data);

private:

    Instruction mips2bin(std::string line, int pc);
    int static_data2bin(std::vector<std::string>* vec_whole_data, std::string line,int pos,int* nob);

    // std::vector<std::string> seg_instruction(std::string line);

    int detect_instruction_type(std::vector<std::string>::iterator funct);

    int determine_rgt_seq_r(std::string opcode);
    int determine_rgt_seq_i(std::string opcode);

};



#endif


