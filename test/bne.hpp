#include "test.hpp"

static bool test_bne_taken(std::string name) {
  uint64_t cycles = 3;

  mem_t memory = { OP_BNE_REL, 0x04 };
  mem_t end_memory = memory;

  Registers start = { .P.Z = 0 };
  Registers end = { .pc = 0x6 };

  return run_test(std::move(name), cycles, memory, end_memory, start, end);
}

static bool test_bne_not_taken(std::string name) {
  uint64_t cycles = 2;

  mem_t memory = { OP_BNE_REL, 0x04 };
  mem_t end_memory = memory;

  Registers start = { .P.Z = 1 };
  Registers end = { .pc = 0x2, .P.Z = 1 };

  return run_test(std::move(name), cycles, memory, end_memory, start, end);
}
