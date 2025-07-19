#include "test.hpp"

static bool test_sta_zpg(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_STA_ZPG, 0xff };
  mem_t end_memory = start_memory;
  end_memory[0xff] = 0x1a;

  Registers start_registers = { .A = 0x1a };
  Registers end_registers = { .A = 0x1a, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sta_zpx(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_STA_ZPX, 0xf1 };
  mem_t end_memory = start_memory;
  end_memory[0xdb] = 0x1a;

  Registers start_registers = { .A = 0x1a, .X = 0xea };
  Registers end_registers = { .A = 0x1a, .X = 0xea, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
