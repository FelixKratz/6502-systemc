#include "test.hpp"

static bool test_ldx_imm(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_LDX_IMM, 0x42 };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .X = 0x42, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldx_imm_zero_flag(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_LDX_IMM, 0x00 };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .X = 0x00, .pc = 0x2, .P.Z = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldx_imm_negative_flag(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_LDX_IMM, 0xFF };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .X = 0xFF, .pc = 0x2, .P.N = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldx_zpg(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_LDX_ZPG, 0x10 };
  start_memory[0x10] = 0x1a;

  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .X = 0x1a, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldx_zpy(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_LDX_ZPY, 0xf0 };
  start_memory[0x0f] = 0x1a;

  mem_t end_memory = start_memory;

  Registers start_registers = { .Y = 0x1f };
  Registers end_registers = { .X = 0x1a, .Y = 0x1f, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldx_abs(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_LDX_ABS, 0x00, 0x40 }; // $4000
  start_memory[0x4000] = 0x77;

  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .X = 0x77, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldx_aby(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_LDX_ABY, 0x00, 0x40 }; // $4000,Y
  start_memory[0x4003] = 0x88;

  mem_t end_memory = start_memory;

  Registers start_registers = { .Y = 0x03 };
  Registers end_registers = { .X = 0x88, .Y = 0x03, .pc = 0x3, .P.N = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_ldx_aby_page_cross(std::string name) {
  uint64_t cycles = 5;

  mem_t start_memory = { OP_LDX_ABY, 0xFF, 0x30 }; // base = $30FF, Y = $01 → $3100
  start_memory[0x3100] = 0x99;

  mem_t end_memory = start_memory;

  Registers start_registers = { .Y = 0x01 };
  Registers end_registers = { .X = 0x99, .Y = 0x01, .pc = 0x3, .P.N = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
