#include "test.hpp"

static bool test_rol_acc(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_ROL_ACC };
  Registers start = { .A = 0x40, .P.C = 1 };
  Registers end = { .A = 0x81, .P.N = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_rol_acc_carry_zero(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_ROL_ACC };
  Registers start = { .A = 0x80, .P.C = 0 };
  Registers end = { .A = 0x00, .P.C = 1, .P.Z = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_rol_zpg(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_ROL_ZPG, 0x10 };
  memory[0x10] = 0x40;
  mem_t expected = memory;
  expected[0x10] = 0x80;
  Registers start = {};
  Registers end = { .P.N = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_rol_zpx(std::string name) {
  uint64_t cycles = 6;
  mem_t memory = { OP_ROL_ZPX, 0x10 };
  memory[0x15] = 0x81;
  mem_t expected = memory;
  expected[0x15] = 0x02;
  Registers start = { .X = 0x05 };
  Registers end = { .X = 0x05, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_rol_abs(std::string name) {
  uint64_t cycles = 6;
  mem_t memory = { OP_ROL_ABS, 0x00, 0x40 }; // $4000
  memory[0x4000] = 0x40;
  mem_t expected = memory;
  expected[0x4000] = 0x80;
  Registers start = {};
  Registers end = { .P.N = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_rol_abx(std::string name) {
  uint64_t cycles = 7;
  mem_t memory = { OP_ROL_ABX, 0x00, 0x40 }; // $4000,X
  memory[0x4005] = 0xFF;
  mem_t expected = memory;
  expected[0x4005] = 0xFE;
  Registers start = { .X = 0x05 };
  Registers end = { .X = 0x05, .P.C = 1, .P.N = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}
