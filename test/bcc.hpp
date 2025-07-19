#include "test.hpp"

static bool test_bcc_taken(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_BCC_REL, 0x05 }; // BCC +5
  mem_t end_memory = start_memory;

  Registers start_registers = { .P = { .C = 0 } };
  Registers end_registers = { .pc = 0x0007, .P = { .C = 0 } };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_bcc_not_taken(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_BCC_REL, 0x05 }; // BCC +5
  mem_t end_memory = start_memory;

  Registers start_registers = { .P = { .C = 1 } };
  Registers end_registers = { .pc = 0x0002, .P = { .C = 1 } };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_bcc_page_cross(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { 0 }; // BCC +127 from 0x00FE -> 0x017F
  start_memory[0x00fd] = OP_BCC_REL;
  start_memory[0x00fe] = 0x7f;
  mem_t end_memory = start_memory;

  Registers start_registers = { .pc = 0x00fd };
  Registers end_registers = { .pc = 0x017e };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

