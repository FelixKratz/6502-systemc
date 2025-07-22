#include "test.hpp"

static bool test_asl_acc(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_ASL_ACC };
  Registers start = { .A = 0x40 };
  Registers end = { .A = 0x80, .P.N = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_asl_acc_carry(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_ASL_ACC };
  Registers start = { .A = 0x80 };
  Registers end = { .A = 0x00, .P.Z = 1, .P.C = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_asl_zpg(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_ASL_ZPG, 0x10 };
  memory[0x10] = 0x22;
  mem_t expected = memory;
  expected[0x10] = 0x44;
  Registers start = {};
  Registers end = { .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_asl_zpx(std::string name) {
  uint64_t cycles = 6;
  mem_t memory = { OP_ASL_ZPX, 0x10 };
  memory[0x15] = 0x81;
  mem_t expected = memory;
  expected[0x15] = 0x02;
  Registers start = { .X = 0x05 };
  Registers end = { .X = 0x05, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_asl_abs(std::string name) {
  uint64_t cycles = 6;
  mem_t memory = { OP_ASL_ABS, 0x00, 0x30 }; // $3000
  memory[0x3000] = 0x40;
  mem_t expected = memory;
  expected[0x3000] = 0x80;
  Registers start = {};
  Registers end = { .P.N = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_asl_abx(std::string name) {
  uint64_t cycles = 7;
  mem_t memory = { OP_ASL_ABX, 0x00, 0x30 }; // $3000,X
  memory[0x3005] = 0xFF;
  mem_t expected = memory;
  expected[0x3005] = 0xFE;
  Registers start = { .X = 0x05 };
  Registers end = { .X = 0x05, .P.C = 1, .P.N = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}
