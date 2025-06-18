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

bool test_sta_zpx(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_STA_ZPX, 0xf1 };
  mem_t end_memory = start_memory;
  end_memory[0xdb] = 0x1a;

  Registers start_registers = { .A = 0x1a, .X = 0xea };
  Registers end_registers = { .A = 0x1a, .X = 0xea, .pc = 0x2 };

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

bool test_lda_zpx(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_LDA_ZPX, 0xf1 };
  start_memory[0xdb] = 0x1a;

  mem_t end_memory = start_memory;

  Registers start_registers = { .X = 0xea};
  Registers end_registers = { .A = 0x1a, .X = 0xea, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

bool test_lda_imm(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_LDA_IMM, 0xff };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .A = 0xff, .pc = 0x2, .P.N = 1 };

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

bool test_adc_imm(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_ADC_IMM, 0xf0 };
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .P.V = 1, .P.C = 1, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

bool test_adc_zpg(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_ADC_ZPG, 0xff };
  start_memory[0xff] = 0xf0;
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .P.V = 1, .P.C = 1, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

bool test_adc_zpx(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_ADC_ZPX, 0xff };
  start_memory[0x40] = 0xf0;
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .X=0x41, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .X=0x41, .P.V = 1, .P.C = 1, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

bool test_adc_abs(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_ADC_ABS, 0x1e, 0xaa };
  start_memory[0xaa1e] = 0xf0;
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .P.V = 1, .P.C = 1, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

bool test_adc_abx(std::string name) {
  uint64_t cycles = 5;

  mem_t start_memory = { OP_ADC_ABX, 0xff, 0xaa };
  start_memory[0xab00] = 0xf0;
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .X = 0x1, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .X = 0x1, .P.V = 1, .P.C = 1, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

bool test_adc_inx(std::string name) {
  uint64_t cycles = 6;

  mem_t start_memory = { OP_ADC_INX, 0xff };
  start_memory[0xa0] = 0xff;
  start_memory[0xff] = 0xf0;
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .X = 0xa1, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .X = 0xa1, .P.V = 1, .P.C = 1, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

bool test_adc_iny(std::string name) {
  uint64_t cycles = 5;

  mem_t start_memory = { OP_ADC_INY, 0xfe };
  start_memory[0xfe] = 0x02;
  start_memory[0xa3] = 0xf0;
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .Y = 0xa1, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .Y = 0xa1, .P.V = 1, .P.C = 1, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

std::vector<TestCase> test_cases = {
  { "jmp_abs", test_jmp_abs },
  { "sta_zpg", test_sta_zpg },
  { "sta_zpx", test_sta_zpx },
  { "lda_zpg", test_lda_zpg },
  { "lda_zpx", test_lda_zpx },
  { "lda_imm", test_lda_imm },
  { "adc_imm", test_adc_imm },
  { "adc_zpg", test_adc_zpg },
  { "adc_abs", test_adc_abs },
  { "adc_abx", test_adc_abx },
  { "adc_zpx", test_adc_zpx },
  { "adc_inx", test_adc_inx },
  { "adc_iny", test_adc_iny },
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
