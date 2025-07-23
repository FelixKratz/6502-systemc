#include "test.hpp"

static bool test_lsr_acc(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_LSR_ACC };
  Registers start = { .A = 0x02 };
  Registers end = { .A = 0x01, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_lsr_acc_carry_zero(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_LSR_ACC };
  Registers start = { .A = 0x01 };
  Registers end = { .A = 0x00, .P.C = 1, .P.Z = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_lsr_zpg(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_LSR_ZPG, 0x10 };
  memory[0x10] = 0x84;
  mem_t expected = memory;
  expected[0x10] = 0x42;
  Registers start = {};
  Registers end = { .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_lsr_zpx(std::string name) {
  uint64_t cycles = 6;
  mem_t memory = { OP_LSR_ZPX, 0x10 };
  memory[0x13] = 0x01;
  mem_t expected = memory;
  expected[0x13] = 0x00;
  Registers start = { .X = 0x03 };
  Registers end = { .X = 0x03, .P.Z = 1, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_lsr_abs(std::string name) {
  uint64_t cycles = 6;
  mem_t memory = { OP_LSR_ABS, 0x00, 0x40 }; // $4000
  memory[0x4000] = 0x22;
  mem_t expected = memory;
  expected[0x4000] = 0x11;
  Registers start = {};
  Registers end = { .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_lsr_abx(std::string name) {
  uint64_t cycles = 7;
  mem_t memory = { OP_LSR_ABX, 0x00, 0x40 }; // $4000,X
  memory[0x4003] = 0x03;
  mem_t expected = memory;
  expected[0x4003] = 0x01;
  Registers start = { .X = 0x03 };
  Registers end = { .X = 0x03, .pc = 0x3, .P.C = 1 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}
