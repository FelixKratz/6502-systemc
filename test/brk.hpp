#include "test.hpp"

static bool test_brk(std::string name) {
  uint64_t cycles = 7;

  mem_t memory = { OP_BRK_IMP };
  memory[0xFFFE] = 0x00; // IRQ vector low byte
  memory[0xFFFF] = 0x40; // IRQ vector high byte

  Registers start = { .pc = 0x1234, .S = 0xFF, .P = { .C = 1,.Z = 0,.I = 0 } };

  mem_t expected = memory;
  expected[0x01FD] = start.P.to_byte() | 0x30;
  expected[0x01FE] = 0x36;
  expected[0x01FF] = 0x12;

  Registers end = { .pc = 0x4000, .S = 0xFC, .P = start.P, };
  end.P.I = 1;

  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_brk_rti_round_trip(std::string name) {
  uint64_t cycles = 13;

  mem_t memory = { OP_BRK_IMP, };
  memory[0xFFFE] = 0x00;
  memory[0xFFFF] = 0x90;

  memory[0x9000] = OP_RTI_IMP;

  Registers start = { .pc = 0x4000, .S  = 0xFF, .P  = { .Z = 1, .C = 1 } };

  mem_t expected = memory;
  expected[0x01FD] = start.P.to_byte() | 0x30;
  expected[0x01FE] = 0x02;
  expected[0x01FF] = 0x40;

  Registers end = { .pc = 0x4002, .S = 0xFF, .P = start.P };

  return run_test(std::move(name), cycles, memory, expected, start, end);
}
