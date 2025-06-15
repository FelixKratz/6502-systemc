#include <systemc>
#include "test.hpp"

bool test_jmp_abs(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_JMP_ABS, 0xff, 0x3e };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .pc = 0x3eff };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

bool test_sta_zpg(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_STA_ZPG, 0xff };
  mem_t end_memory = start_memory;
  end_memory[0xff] = 0x1a;

  Registers start_registers = { .A = 0x1a };
  Registers end_registers = { .A = 0x1a, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

bool test_lda_zpg(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_LDA_ZPG, 0xff };
  start_memory[0xff] = 0x1a;

  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .A = 0x1a, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

bool test_lda_imm(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_LDA_IMM, 0xff };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .A = 0xff, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

bool test_nop(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_NOP };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}



std::vector<TestCase> test_cases = {
  { "jmp_abs", test_jmp_abs },
  { "sta_zpg", test_sta_zpg },
  { "lda_zpg", test_lda_zpg },
  { "lda_imm", test_lda_imm },
  { "nop",     test_nop },
};

int sc_main(int argc, char* argv[]) {
  if (argc == 2) {
    int test_case = 0;
    char name[8] = {};

    if (sscanf(argv[1], "test_case=%d", &test_case) == 1) {
      if (test_case >= 0
          && test_case < test_cases.size()) {
        return !test_cases[test_case].function(test_cases[test_case].name);
      } 
    } else if (sscanf(argv[1], "test_case=%7s", name) == 1) {
      for (size_t i = 0; i < test_cases.size(); i++) {
        if (test_cases[i].name == name) {
          return !test_cases[i].function(test_cases[i].name);
        }
      } 
      std::cout << "Test case: " << name << " not found." << std::endl;
      return 1;
    }  else if (strncmp(argv[1], "--count", 7) == 0) {
      std::cout << test_cases.size() << std::endl;
      return 0;
    }
  }

  std::cout << "Usage: <bin> --count | test_case=<integer> " << std::endl;
  std::cout << "Available Test Cases:" << std::endl;
  for (size_t i = 0; i < test_cases.size(); ++i) {
    std::cout << "  [" << i << "] " << test_cases[i].name << "\n";
  }
  return 0;
}
