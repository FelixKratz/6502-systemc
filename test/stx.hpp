#include "test.hpp"

static bool test_stx_zpg(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_STX_ZPG, 0x42 };
  mem_t end_memory = start_memory;
  end_memory[0x42] = 0x99;

  Registers start_registers = { .X = 0x99 };
  Registers end_registers = { .X = 0x99, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_stx_abs(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_STX_ABS, 0x00, 0x30 }; // STX $3000
  mem_t end_memory = start_memory;
  end_memory[0x3000] = 0x55;

  Registers start_registers = { .X = 0x55 };
  Registers end_registers = { .X = 0x55, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_stx_zpy(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_STX_ZPY, 0x40 }; // STX $40,Y
  mem_t end_memory = start_memory;
  end_memory[0x44] = 0x77;

  Registers start_registers = { .X = 0x77, .Y = 0x04 };
  Registers end_registers = { .X = 0x77, .Y = 0x04, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
