#include "test.hpp"

static bool test_adc_imm(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_ADC_IMM, 0xf0 };
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .P.V = 1, .P.C = 1, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_adc_zpg(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_ADC_ZPG, 0xff };
  start_memory[0xff] = 0xf0;
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .P.V = 1, .P.C = 1, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_adc_zpx(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_ADC_ZPX, 0xff };
  start_memory[0x40] = 0xf0;
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .X=0x41, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .X=0x41, .P.V = 1, .P.C = 1, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_adc_abs(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_ADC_ABS, 0x1e, 0xaa };
  start_memory[0xaa1e] = 0xf0;
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .P.V = 1, .P.C = 1, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_adc_abx(std::string name) {
  uint64_t cycles = 5;

  mem_t start_memory = { OP_ADC_ABX, 0xff, 0xaa };
  start_memory[0xab00] = 0xf0;
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .X = 0x1, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .X = 0x1, .P.V = 1, .P.C = 1, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_adc_inx(std::string name) {
  uint64_t cycles = 6;

  mem_t start_memory = { OP_ADC_INX, 0xff };
  start_memory[0xa0] = 0xff;
  start_memory[0xff] = 0xf0;
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .X = 0xa1, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .X = 0xa1, .P.V = 1, .P.C = 1, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_adc_iny(std::string name) {
  uint64_t cycles = 5;

  mem_t start_memory = { OP_ADC_INY, 0xfe };
  start_memory[0xfe] = 0x02;
  start_memory[0xa3] = 0xf0;
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80, .Y = 0xa1, .P.C = 1 };
  Registers end_registers = { .A = 0x71, .Y = 0xa1, .P.V = 1, .P.C = 1, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
