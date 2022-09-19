#ifndef SIMULATOR_H_
#define SIMULATOR_H_

class Simulator
{
public:
    // constructor
    Simulator(int num_of_byte);

    void simulate(uint32_t* pc_ptr, uint32_t* pc_start, uint32_t* memory, std::ifstream& syscall_inputs, std::ofstream& output_file);

    void show_registers();

// private:
    uint32_t rgt[32];
    uint32_t hi;
    uint32_t lo;
    uint32_t rgt_pc;
    uint32_t dm_end;

    void exe_funct(int type, uint32_t*& pt, uint32_t i_rs, uint32_t i_rt, uint32_t i_rd, 
                          std::string s_shamt, std::string s_imm, std::string s_add, uint32_t* memory, 
                          std::ifstream& syscall_inputs, std::ofstream& output_file);

    int rec_instr(std::string code);
};

#endif