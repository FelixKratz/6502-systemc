#include "test.hpp"

static bool test_lda_abx(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_LDA_ABX, 0x00, 0x30 }; // LDA $3000,X
  start_memory[0x3005] = 0x42;

  mem_t end_memory = start_memory;

  Registers start_registers = { .X = 0x05 };
  Registers end_registers = { .A = 0x42, .X = 0x05, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_lda_aby(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_LDA_ABY, 0x00, 0x30 }; // LDA $3000,Y
  start_memory[0x3005] = 0x42;

  mem_t end_memory = start_memory;

  Registers start_registers = { .Y = 0x05 };
  Registers end_registers = { .A = 0x42, .Y = 0x05, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_lda_iny(std::string name) {
  uint64_t cycles = 5;

  mem_t start_memory = { OP_LDA_INY, 0x10 }; // LDA ($10),Y
  start_memory[0x10] = 0x00;
  start_memory[0x11] = 0x30;
  start_memory[0x3005] = 0x42;

  mem_t end_memory = start_memory;

  Registers start_registers = { .Y = 0x05 };
  Registers end_registers = { .A = 0x42, .Y = 0x05, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_lda_abs(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_LDA_ABS, 0x00, 0x30 }; // LDA $3000
  start_memory[0x3000] = 0x42;

  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .A = 0x42, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_lda_inx(std::string name) {
  uint64_t cycles = 6;

  mem_t start_memory = { OP_LDA_INX, 0x10 };
  start_memory[0x14] = 0x00;
  start_memory[0x15] = 0x30;
  start_memory[0x3000] = 0x42;

  mem_t end_memory = start_memory;

  Registers start_registers = { .X = 0x04 };
  Registers end_registers = { .A = 0x42, .X = 0x04, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_lda_zpg(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_LDA_ZPG, 0xff };
  start_memory[0xff] = 0x1a;

  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .A = 0x1a, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_lda_zpx(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_LDA_ZPX, 0xf1 };
  start_memory[0xdb] = 0x1a;

  mem_t end_memory = start_memory;

  Registers start_registers = { .X = 0xea};
  Registers end_registers = { .A = 0x1a, .X = 0xea, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_lda_imm(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_LDA_IMM, 0xff };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .A = 0xff, .pc = 0x2, .P.N = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_lda_imm_zero_flag(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_LDA_IMM, 0x00 };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .A = 0x00, .pc = 0x2, .P.Z = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_lda_abx_page_cross(std::string name) {
  uint64_t cycles = 5;

  mem_t start_memory = { OP_LDA_ABX, 0xFD, 0x30 }; // base = $30FD, X = $03 → $3100
  start_memory[0x3100] = 0x99;

  mem_t end_memory = start_memory;

  Registers start_registers = { .X = 0x03 };
  Registers end_registers = { .A = 0x99, .X = 0x03, .pc = 0x3, .P.N = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}


static bool test_lda_iny_page_cross(std::string name) {
  uint64_t cycles = 6;

  mem_t start_memory = { OP_LDA_INY, 0x10 }; // ($10),Y
  start_memory[0x10] = 0xFE;
  start_memory[0x11] = 0x30;
  start_memory[0x31FD] = 0x88;

  mem_t end_memory = start_memory;

  Registers start_registers = { .Y = 0xFF };
  Registers end_registers = { .A = 0x88, .Y = 0xFF, .pc = 0x2, .P.N = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
