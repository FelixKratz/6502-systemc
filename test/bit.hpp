#include "test.hpp"

static bool test_bit_zpg_zero(std::string name) {
  uint64_t cycles = 3;
  mem_t memory = { OP_BIT_ZPG, 0x10 };
  memory[0x10] = 0x00;
  Registers start = { .A = 0xFF };
  Registers end = { .A = 0xFF, .P.Z = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_bit_zpg_negative(std::string name) {
  uint64_t cycles = 3;
  mem_t memory = { OP_BIT_ZPG, 0x10 };
  memory[0x10] = 0x80; // N = 1
  Registers start = { .A = 0xFF };
  Registers end = { .A = 0xFF, .P.N = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_bit_zpg_overflow(std::string name) {
  uint64_t cycles = 3;
  mem_t memory = { OP_BIT_ZPG, 0x10 };
  memory[0x10] = 0x40; // V = 1
  Registers start = { .A = 0xFF };
  Registers end = { .A = 0xFF, .P.V = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_bit_abs_zero(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_BIT_ABS, 0x00, 0x40 };
  memory[0x4000] = 0x00;
  Registers start = { .A = 0xAA };
  Registers end = { .A = 0xAA, .P.Z = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_bit_abs_negative_overflow(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_BIT_ABS, 0x00, 0x40 };
  memory[0x4000] = 0xC0; // N = 1, V = 1
  Registers start = { .A = 0xFF };
  Registers end = { .A = 0xFF, .P.N = 1, .P.V = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}
