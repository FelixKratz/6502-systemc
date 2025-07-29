#include "test.hpp"

static bool test_beq_taken(std::string name) {
  uint64_t cycles = 3;

  mem_t memory = { OP_BEQ_REL, 0x04 };
  mem_t end_memory = memory;

  Registers start = { .P.Z = 1 };
  Registers end = { .pc = 0x6, .P.Z = 1 };

  return run_test(std::move(name), cycles, memory, end_memory, start, end);
}

static bool test_beq_not_taken(std::string name) {
  uint64_t cycles = 2;

  mem_t memory = { OP_BEQ_REL, 0x04 };
  mem_t end_memory = memory;

  Registers start = { .P.Z = 0 };
  Registers end = { .pc = 0x2 };

  return run_test(std::move(name), cycles, memory, end_memory, start, end);
}

static bool test_beq_page_cross_taken(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { 0 };
  memory[0x01FD] = OP_BEQ_REL;
  memory[0x01FE] = 0x1;

  Registers start = { .pc = 0x01FD, .P.Z = 1 };
  Registers end   = { .pc = 0x0200, .P.Z = 1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}
