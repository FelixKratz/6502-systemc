#include "test.hpp"

static bool test_bcs_taken(std::string name) {
  uint64_t cycles = 3;

  mem_t memory = { OP_BCS_REL, 0x04 };
  mem_t end_memory = memory;

  Registers start = { .P.C = 1 };
  Registers end = { .pc = 0x6, .P.C = 1 };

  return run_test(std::move(name), cycles, memory, end_memory, start, end);
}

static bool test_bcs_not_taken(std::string name) {
  uint64_t cycles = 2;

  mem_t memory = { OP_BCS_REL, 0x04 };
  mem_t end_memory = memory;

  Registers start = { .P.C = 0 };
  Registers end = { .pc = 0x2 };

  return run_test(std::move(name), cycles, memory, end_memory, start, end);
}
