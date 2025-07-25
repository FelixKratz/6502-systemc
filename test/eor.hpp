#include "test.hpp"

static bool test_eor_imm(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_EOR_IMM, 0xF0 };
  Registers start = { .A = 0xCC };
  Registers end = { .A = 0x3C, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_eor_imm_zero(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_EOR_IMM, 0xFF };
  Registers start = { .A = 0xFF };
  Registers end = { .A = 0x00, .P.Z = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_eor_imm_negative(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_EOR_IMM, 0x80 };
  Registers start = { .A = 0x00 };
  Registers end = { .A = 0x80, .P.N = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_eor_zpg(std::string name) {
  uint64_t cycles = 3;
  mem_t memory = { OP_EOR_ZPG, 0x10 };
  memory[0x10] = 0xF0;
  Registers start = { .A = 0xCC };
  Registers end = { .A = 0x3C, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_eor_zpx(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_EOR_ZPX, 0x10 };
  memory[0x15] = 0x0F;
  Registers start = { .A = 0xF0, .X = 0x05 };
  Registers end = { .A = 0xFF, .X = 0x05, .P.N = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_eor_abs(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_EOR_ABS, 0x00, 0x40 };
  memory[0x4000] = 0xAA;
  Registers start = { .A = 0xFF };
  Registers end = { .A = 0x55, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_eor_abx(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_EOR_ABX, 0x00, 0x40 };
  memory[0x4005] = 0x0F;
  Registers start = { .A = 0xF0, .X = 0x05 };
  Registers end = { .A = 0xFF, .X = 0x05, .P.N = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_eor_aby(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_EOR_ABY, 0x00, 0x40 };
  memory[0x4003] = 0x88;
  Registers start = { .A = 0xF8, .Y = 0x03 };
  Registers end = { .A = 0x70, .Y = 0x03, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_eor_inx(std::string name) {
  uint64_t cycles = 6;
  mem_t memory = { OP_EOR_INX, 0x10 };
  memory[0x14] = 0x00;
  memory[0x15] = 0x30;
  memory[0x3000] = 0x55;
  Registers start = { .A = 0xF0, .X = 0x04 };
  Registers end = { .A = 0xA5, .X = 0x04, .pc = 0x2, .P.N = 1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_eor_iny(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_EOR_INY, 0x10 };
  memory[0x10] = 0x00;
  memory[0x11] = 0x40;
  memory[0x4005] = 0x0F;
  Registers start = { .A = 0xF0, .Y = 0x05 };
  Registers end = { .A = 0xFF, .Y = 0x05, .P.N = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}
