#include "test.hpp"

static bool test_sty_zpg(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_STY_ZPG, 0x42 };
  mem_t end_memory = start_memory;
  end_memory[0x42] = 0x2a;

  Registers start_registers = { .Y = 0x2a };
  Registers end_registers = { .Y = 0x2a, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sty_abs(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_STY_ABS, 0x00, 0x30 }; // STY $3000
  mem_t end_memory = start_memory;
  end_memory[0x3000] = 0x2a;

  Registers start_registers = { .Y = 0x2a };
  Registers end_registers = { .Y = 0x2a, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sty_zpx(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_STY_ZPX, 0xf1 };
  mem_t end_memory = start_memory;
  end_memory[0xdb] = 0x2a;

  Registers start_registers = { .Y = 0x2a, .X = 0xea };
  Registers end_registers = { .Y = 0x2a, .X = 0xea, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
