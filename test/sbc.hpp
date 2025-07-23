#include "test.hpp"

static bool test_sbc_imm(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_SBC_IMM, 0x10 };
  Registers start = { .A = 0x80, .P.C = 1 };
  Registers end = { .A = 0x70, .P.V = 1, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_zpg(std::string name) {
  uint64_t cycles = 3;
  mem_t memory = { OP_SBC_ZPG, 0x42 };
  memory[0x42] = 0x10;
  Registers start = { .A = 0x80, .P.C = 1 };
  Registers end = { .A = 0x70, .P.V = 1, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_zpx(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_SBC_ZPX, 0x3F };
  memory[0x44] = 0x10;
  Registers start = { .A = 0x80, .X = 0x05, .P.C = 1 };
  Registers end = { .A = 0x70, .X = 0x05, .P.V = 1, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_abs(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_SBC_ABS, 0x00, 0x20 };
  memory[0x2000] = 0x10;
  Registers start = { .A = 0x80, .P.C = 1 };
  Registers end = { .A = 0x70, .P.V = 1, .P.C = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_abx(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_SBC_ABX, 0xFF, 0x30 };
  memory[0x3100] = 0x10;
  Registers start = { .A = 0x80, .X = 0x01, .P.C = 1 };
  Registers end = { .A = 0x70, .X = 0x01, .P.V = 1, .P.C = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_aby(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_SBC_ABY, 0xFD, 0x20 };
  memory[0x2100] = 0x10;
  Registers start = { .A = 0x80, .Y = 0x03, .P.C = 1 };
  Registers end = { .A = 0x70, .Y = 0x03, .P.V = 1, .P.C = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_inx(std::string name) {
  uint64_t cycles = 6;
  mem_t memory = { OP_SBC_INX, 0x20 };
  memory[0x25] = 0x00;
  memory[0x26] = 0x40;
  memory[0x4000] = 0x10;
  Registers start = { .A = 0x80, .X = 0x05, .P.C = 1 };
  Registers end = { .A = 0x70, .X = 0x05, .P.V = 1, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_iny(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_SBC_INY, 0x10 };
  memory[0x10] = 0x00;
  memory[0x11] = 0x30;
  memory[0x30FF] = 0x10;
  Registers start = { .A = 0x80, .Y = 0xFF, .P.C = 1 };
  Registers end = { .A = 0x70, .Y = 0xFF, .P.V = 1, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_imm_borrow(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_SBC_IMM, 0x01 };
  Registers start = { .A = 0x00, .P.C = 0 };
  Registers end = { .A = 0xFE, .P.N = 1, .P.C = 0, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_imm_no_borrow(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_SBC_IMM, 0x01 };
  Registers start = { .A = 0x02, .P.C = 1 };
  Registers end = { .A = 0x01, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_imm_zero(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_SBC_IMM, 0x01 };
  Registers start = { .A = 0x01, .P.C = 1 };
  Registers end = { .A = 0x00, .P.Z = 1, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_imm_negative(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_SBC_IMM, 0x01 };
  Registers start = { .A = 0x00, .P.C = 1 };
  Registers end = { .A = 0xFF, .P.N = 1, .P.C = 0, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_imm_overflow(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_SBC_IMM, 0xFF }; // 0x7F - 0xFF - !C = 0x7F - 0xFF - 1 = 0x7F - 0x100 = -0x81
  Registers start = { .A = 0x7F, .P.C = 1 };
  Registers end = { .A = 0x80, .P.V = 1, .P.N = 1, .P.C = 0, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_abs_page_cross(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_SBC_ABX, 0xFF, 0x20 };
  memory[0x2100] = 0x01;
  Registers start = { .A = 0x03, .X = 0x01, .P.C = 1 };
  Registers end = { .A = 0x02, .X = 0x01, .P.C = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_sbc_iny_page_cross(std::string name) {
  uint64_t cycles = 6;
  mem_t memory = { OP_SBC_INY, 0x10 };
  memory[0x10] = 0xFF;
  memory[0x11] = 0x20;
  memory[0x21FE] = 0x01;
  Registers start = { .A = 0x05, .Y = 0xFF, .P.C = 1 };
  Registers end = { .A = 0x04, .Y = 0xFF, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}
