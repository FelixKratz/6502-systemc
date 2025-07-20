#include "test.hpp"

static bool test_bmi_taken(std::string name) {
  uint64_t cycles = 3;

  mem_t memory = { OP_BMI_REL, 0x04 };
  mem_t end_memory = memory;

  Registers start = { .P.N = 1 };
  Registers end = { .pc = 0x6, .P.N = 1 };

  return run_test(std::move(name), cycles, memory, end_memory, start, end);
}

static bool test_bmi_not_taken(std::string name) {
  uint64_t cycles = 2;

  mem_t memory = { OP_BMI_REL, 0x04 };
  mem_t end_memory = memory;

  Registers start = { .P.N = 0 };
  Registers end = { .pc = 0x2 };

  return run_test(std::move(name), cycles, memory, end_memory, start, end);
}
