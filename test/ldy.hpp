#include "test.hpp"

static bool test_ldy_imm(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_LDY_IMM, 0x80 };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .Y = 0x80, .pc = 0x2, .P.N = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldy_imm_zero_flag(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_LDY_IMM, 0x00 };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .Y = 0x00, .pc = 0x2, .P.Z = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldy_zpg(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_LDY_ZPG, 0x20 };
  start_memory[0x20] = 0x1a;

  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .Y = 0x1a, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldy_zpx(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_LDY_ZPX, 0xf0 };
  start_memory[0xfa] = 0x42;

  mem_t end_memory = start_memory;

  Registers start_registers = { .X = 0x0A };
  Registers end_registers = { .Y = 0x42, .X = 0x0A, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldy_abs(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_LDY_ABS, 0x34, 0x12 }; // $1234
  start_memory[0x1234] = 0x99;

  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .Y = 0x99, .pc = 0x3, .P.N = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldy_abx(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_LDY_ABX, 0x00, 0x40 }; // $4000,X
  start_memory[0x4003] = 0x77;

  mem_t end_memory = start_memory;

  Registers start_registers = { .X = 0x03 };
  Registers end_registers = { .Y = 0x77, .X = 0x03, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldy_abx_page_cross(std::string name) {
  uint64_t cycles = 5;

  mem_t start_memory = { OP_LDY_ABX, 0xFD, 0x30 }; // base = $30FD, X = $03 → $3100
  start_memory[0x3100] = 0xFF;

  mem_t end_memory = start_memory;

  Registers start_registers = { .X = 0x03 };
  Registers end_registers = { .Y = 0xFF, .X = 0x03, .pc = 0x3, .P.N = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
