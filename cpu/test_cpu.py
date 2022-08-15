# 1. make the program
# 2. write into Instruction.txt
# 3. execute the program
# 4. test one test case
# 5. test all test cases


import argparse
import os
import sys

argparser = argparse.ArgumentParser(prog='test_cpu')
argparser.add_argument('-m', '--make', action='store_true', help='Compile the cpu program')
argparser.add_argument('-l', '--link', type=int, help='Soft link the tested instructions into Instruction.txt, to facilitate test')
argparser.add_argument('-e', '--execute', action='store_true', help='Execute the cpu program')
argparser.add_argument('-c', '--case', type=int, help='Test one test case, should input a number to indicate which test case to be tested')
argparser.add_argument('-a', '--all', action='store_true', help='Test all test cases')


def main(argv=sys.argv[1:]):
    args = argparser.parse_args(argv)

    if args.make:
        make()
    if args.link:
        sl_instruction(args.link)
    if args.execute:
        execute()
    if args.case:
        compare_DataRAM(args.case)
    if args.all:
        run_all_test_cases()
        



def make():
    """Compile the CPU program 

    Remove the result of last compilation, if exists. Then, compile the CPU program.

    """

    system_operations = """
        cd cpu;
        if [ -f test_cpu.out ];then
            rm test_cpu.out
        fi
        make;
        cd ..
    """

    os.system(system_operations)


def sl_instruction(testcase_num):
    """
    Soft link test case machine code into instruction.txt to facilitate testing.
    """

    system_operations = """
        ln -f ./cpu_test/machine_code{}.txt ./cpu_test/machine_code.txt
    """.format(testcase_num)

    os.system(system_operations)



def execute():
    """Execute the CPU program 

    Execute the compiled CPU program.

    """

    system_operations = """
        cd cpu;
        ./test_cpu.out; 
        cd ..
    """

    os.system(system_operations)

def compare_DataRAM(testcase_num):
    """Compares the result of DataRAM and DataRAM in test cases

    If the two files are similiar, prints "🎉pass test case"
    Else, prints "🐞fail test case"

    Args:
        The index of the test case.

    Returns:
        NULL
    """

    res_path = "./cpu/out.bin"
    ans_path = "./cpu_test/DATA_RAM{}.txt".format(testcase_num)

    res = open(res_path, 'r').readlines()
    ans = open(ans_path, 'r').readlines()

    diff = set(ans)-set(res)
    if (diff.__len__() == 0):
        print("🎉pass test case {}".format(testcase_num))
    else:
        print("🐞fail test case {}".format(testcase_num))


def run_all_test_cases():
    for testcase_i in range(1,9):
        make()
        sl_instruction(testcase_i)
        execute()
        compare_DataRAM(testcase_i)
    


def bin2hex(testcase_num):
    """Convert binary number into hexadecimal number

    Used to convert instruction from binary into hexadecimal,
    to ease comparison

    Args:
        The index of the test case.

    Returns:
        String contains the instructions in hexadecimal format
    """

    path = "./cpu_test/machine_code{}.txt".format(testcase_num)

    with open(path,'r') as f:
        res_hex = ""
        for line in f:
            inst_hex = hex(int(line, 2))
            res_hex = res_hex + inst_hex + "\n"
    
    return(res_hex)



if __name__ == "__main__":
    main()


