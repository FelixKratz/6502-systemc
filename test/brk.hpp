#include "test.hpp"

static bool test_brk(std::string name) {
  uint64_t cycles = 7;

  mem_t memory = { OP_BRK_IMP };
  memory[0xFFFE] = 0x00; // IRQ vector low byte
  memory[0xFFFF] = 0x40; // IRQ vector high byte

  Registers start = { .pc = 0x1234, .S = 0xFF, .P = { .C = 1,.Z = 0,.I = 0 } };

  mem_t expected = memory;
  expected[0x01FF] = 0x36;
  expected[0x01FE] = 0x12;
  expected[0x01FD] = start.P.to_byte() | 0x30;

  Registers end = { .pc = 0x4000, .S = 0xFC, .P = start.P, };
  end.P.I = 1;

  return run_test(std::move(name), cycles, memory, expected, start, end);
}
