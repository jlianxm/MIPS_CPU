#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iterator>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "utils.h"
#include "scanner.h"
#include "simulator.h"


int main(int argc, char**argv) {
    if(argc<4){
        printf("wrong number of arguments");
    }

    // open files
    std::ifstream input_mips;
    input_mips.open(argv[1]);
    std::ifstream syscall_inputs;
    syscall_inputs.open(argv[2]);
    std::ofstream output_file;
    output_file.open(argv[3]);
    
    // allocate memory
    uint32_t *memory;
    memory=(uint32_t *)malloc(0xa00000*sizeof(uint32_t));
    
    // assign pointers to mark the start and current position of the code and the static data
    uint32_t* pc_ptr = &memory[0x100000]; // current position
    uint32_t* pc_start = pc_ptr; // start position
    uint32_t* data_ptr = &memory[0x140000]; // current position
    uint32_t* data_start = data_ptr; // start position

    // read the MIPS program
    std::vector<std::string> vec_input_raw;
    std::string line_raw;
    while (std::getline(input_mips,line_raw)) {
        vec_input_raw.push_back(line_raw);
    }

    Scanner scanner;
    // preprocess
    scanner.preprocess(vec_input_raw);

    #ifdef DEBUG
        // data section
        for (std::vector<std::string>::iterator iter = scanner.vec_data.begin(); iter != scanner.vec_data.end(); ++iter)
        {
            std::cout << *iter << std::endl;
        }

        // code section
        for (std::vector<std::string>::iterator iter = scanner.vec_text.begin(); iter != scanner.vec_text.end(); ++iter)
        {
            std::cout << *iter << std::endl;
        }

    #endif
    

    // store mips code and static data into the pseudo main memory
    scanner.store_machine_code(pc_ptr, scanner.vec_text);
    int num_of_byte = scanner.store_static_data(data_ptr, scanner.vec_data);

    #ifdef DEBUG
        // data section
        for (uint32_t* pt = data_start; pt != data_ptr; ++pt)
        {   
            std::cout << "pt = " << pt << "; *pt = " << *pt << std::endl;
        }

        // code section

        // label map
        for (std::map<std::string,int>::iterator iter = scanner.label.begin(); iter != scanner.label.end(); ++iter)
        {
            std::cout << iter->first << " " << iter->second << std::endl;
        }

    #endif

    // simulate the execution of the program
    Simulator simulator(num_of_byte);

    simulator.simulate(pc_ptr, pc_start, memory, syscall_inputs, output_file);

    std::cout << "finished!!" << std::endl;

    return 0;
}




