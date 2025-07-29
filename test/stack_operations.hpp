#include "test.hpp"

static bool test_pha(std::string name) {
  uint64_t cycles = 3;

  mem_t memory = { OP_PHA_IMP };
  Registers start = { .A = 0x42, .S = 0xFD };
  mem_t expected = memory;
  expected[0x01FD] = 0x42;

  Registers end = { .A = 0x42, .S = 0xFC, .pc = 0x1 };

  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_pla(std::string name) {
  uint64_t cycles = 4;

  mem_t memory = { OP_PLA_IMP };
  memory[0x01FE] = 0x37;

  Registers start = { .S = 0xFD };
  Registers end = { .A = 0x37, .S = 0xFE, .pc = 0x1 };

  mem_t expected = memory;

  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_php(std::string name) {
  uint64_t cycles = 3;

  mem_t memory = { OP_PHP_IMP };
  Registers start = { .P = {.C = 1, .Z = 0, .I = 1}, .S = 0xFC };
  mem_t expected = memory;
  expected[0x01FC] = start.P.to_byte() | 0x30;

  Registers end = start;
  end.S = 0xFB;
  end.pc = 0x1;

  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_plp(std::string name) {
  uint64_t cycles = 4;

  mem_t memory = { OP_PLP_IMP };
  memory[0x01FD] = 0b10100101;

  Registers start = { .S = 0xFC };
  Registers end = { .S = 0xFD, .pc = 0x1 };
  end.P.from_byte(0b10100101);

  mem_t expected = memory;

  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_jsr(std::string name) {
  uint64_t cycles = 6;

  mem_t memory = { OP_JSR_ABS, 0x00, 0x80 };
  mem_t expected = memory;
  expected[0x01FD] = 0x02;
  expected[0x01FC] = 0x00;

  Registers start = { .pc = 0x0000, .S = 0xFE };
  Registers end = { .pc = 0x8000, .S = 0xFC };

  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_rts(std::string name) {
  uint64_t cycles = 6;

  mem_t memory = { OP_RTS_IMP };
  memory[0x01FD] = 0x02;
  memory[0x01FE] = 0x40;

  Registers start = { .S = 0xFC };
  Registers end = { .pc = 0x4003, .S = 0xFE };

  mem_t expected = memory;

  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_rti(std::string name) {
  uint64_t cycles = 6;

  mem_t memory = { OP_RTI_IMP };
  memory[0x01FD] = 0b10100101; 
  memory[0x01FE] = 0x56;
  memory[0x01FF] = 0x34;

  Registers start = { .S = 0xFC };
  Registers end = { .pc = 0x3456, .S = 0xFF };
  end.P.from_byte(0b10100101);

  return run_test(std::move(name), cycles, memory, memory, start, end);
}
