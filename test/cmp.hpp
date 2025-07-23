#include "test.hpp"

static bool test_cmp_imm(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_CMP_IMM, 0x10 };
  Registers start = { .A = 0x20 };
  Registers end = { .A = 0x20, .P.N = 0, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cmp_zpg(std::string name) {
  uint64_t cycles = 3;
  mem_t memory = { OP_CMP_ZPG, 0x42 };
  memory[0x42] = 0x10;
  Registers start = { .A = 0x20 };
  Registers end = { .A = 0x20, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cmp_zpx(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_CMP_ZPX, 0x3F };
  memory[0x44] = 0x20;
  Registers start = { .A = 0x20, .X = 0x05 };
  Registers end = { .A = 0x20, .X = 0x05, .P.Z = 1, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cmp_abs(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_CMP_ABS, 0x00, 0x20 };
  memory[0x2000] = 0x30;
  Registers start = { .A = 0x20 };
  Registers end = { .A = 0x20, .P.N = 1, .P.C = 0, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cmp_abx(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_CMP_ABX, 0x00, 0x30 };
  memory[0x3002] = 0x10;
  Registers start = { .A = 0x10, .X = 0x02 };
  Registers end = { .A = 0x10, .X = 0x02, .P.Z = 1, .P.C = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cmp_aby(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_CMP_ABY, 0x00, 0x40 };
  memory[0x4003] = 0x05;
  Registers start = { .A = 0x04, .Y = 0x03 };
  Registers end = { .A = 0x04, .Y = 0x03, .P.N = 1, .P.C = 0, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cmp_inx(std::string name) {
  uint64_t cycles = 6;
  mem_t memory = { OP_CMP_INX, 0x20 };
  memory[0x25] = 0x00;
  memory[0x26] = 0x50;
  memory[0x5000] = 0x01;
  Registers start = { .A = 0x01, .X = 0x05 };
  Registers end = { .A = 0x01, .X = 0x05, .P.Z = 1, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cmp_iny(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_CMP_INY, 0x10 };
  memory[0x10] = 0x00;
  memory[0x11] = 0x60;
  memory[0x6002] = 0x03;
  Registers start = { .A = 0x03, .Y = 0x02 };
  Registers end = { .A = 0x03, .Y = 0x02, .P.Z = 1, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cmp_imm_greater_than(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_CMP_IMM, 0x10 };
  Registers start = { .A = 0x11 };
  Registers end = { .A = 0x11, .P.N = 0, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cmp_imm_equal(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_CMP_IMM, 0x10 };
  Registers start = { .A = 0x10 };
  Registers end = { .A = 0x10, .P.Z = 1, .P.N = 0, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cmp_imm_less_than(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_CMP_IMM, 0x10 };
  Registers start = { .A = 0x0F };
  Registers end = { .A = 0x0F, .P.N = 1, .P.C = 0, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}
