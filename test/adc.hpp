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

static bool test_adc_imm_overflow(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_ADC_IMM, 0x7f }; // A = 0x01 + 0x7f = 0x80 (overflow)
  Registers start = { .A = 0x01, .P.C = 0 };
  Registers end = { .A = 0x80, .P.V = 1, .P.N = 1, .P.Z = 0, .P.C = 0, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_adc_imm_zero_carry(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_ADC_IMM, 0x00 };
  Registers start = { .A = 0x00, .P.C = 0 };
  Registers end = { .A = 0x00, .P.Z = 1, .P.N = 0, .P.C = 0, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_adc_abs_page_cross(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_ADC_ABX, 0xff, 0x30 }; // base = 0x30FF + 1 = 0x3100
  memory[0x3100] = 0x01;
  Registers start = { .A = 0x01, .X = 0x01 };
  Registers end = { .A = 0x02, .P.Z = 0, .P.C = 0, .pc = 0x3, .X = 0x01 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_adc_iny_page_cross(std::string name) {
  uint64_t cycles = 6;
  mem_t memory = { OP_ADC_INY, 0x10 };
  memory[0x10] = 0xFF;
  memory[0x11] = 0x30;
  memory[0x31FE] = 0x05;
  Registers start = { .A = 0x03, .Y = 0xFF };
  Registers end = { .A = 0x08, .pc = 0x2, .Y = 0xFF };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_adc_zpg_carry_out(std::string name) {
  uint64_t cycles = 3;
  mem_t memory = { OP_ADC_ZPG, 0x10 };
  memory[0x10] = 0xFF;
  Registers start = { .A = 0x02, .P.C = 1 };
  Registers end = { .A = 0x02, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}
