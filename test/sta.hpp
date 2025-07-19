#include "test.hpp"
#include "test.hpp"

static bool test_sta_zpg(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_STA_ZPG, 0xff };
  mem_t end_memory = start_memory;
  end_memory[0xff] = 0x1a;

  Registers start_registers = { .A = 0x1a };
  Registers end_registers = { .A = 0x1a, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sta_zpx(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_STA_ZPX, 0xf1 };
  mem_t end_memory = start_memory;
  end_memory[0xdb] = 0x1a;

  Registers start_registers = { .A = 0x1a, .X = 0xea };
  Registers end_registers = { .A = 0x1a, .X = 0xea, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sta_abs(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_STA_ABS, 0x00, 0x30 }; // sta $3000
  mem_t end_memory = start_memory;
  end_memory[0x3000] = 0x2a;

  Registers start_registers = { .A = 0x2a };
  Registers end_registers = { .A = 0x2a, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sta_abx(std::string name) {
  uint64_t cycles = 5;

  mem_t start_memory = { OP_STA_ABX, 0x00, 0x30 }; // sta $3000,X
  mem_t end_memory = start_memory;
  end_memory[0x3005] = 0x2a;

  Registers start_registers = { .A = 0x2a, .X = 0x05 };
  Registers end_registers = { .A = 0x2a, .X = 0x05, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sta_aby(std::string name) {
  uint64_t cycles = 5;

  mem_t start_memory = { OP_STA_ABY, 0x00, 0x30 }; // sta $3000,Y
  mem_t end_memory = start_memory;
  end_memory[0x3005] = 0x2a;

  Registers start_registers = { .A = 0x2a, .Y = 0x05 };
  Registers end_registers = { .A = 0x2a, .Y = 0x05, .pc = 0x3 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sta_inx(std::string name) {
  uint64_t cycles = 6;

  mem_t start_memory = { OP_STA_INX, 0x10 }; // sta ($10,X)
  start_memory[0x14] = 0x00;
  start_memory[0x15] = 0x40;

  mem_t end_memory = start_memory;
  end_memory[0x4000] = 0x2a;

  Registers start_registers = { .A = 0x2a, .X = 0x04 };
  Registers end_registers = { .A = 0x2a, .X = 0x04, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sta_iny(std::string name) {
  uint64_t cycles = 6;

  mem_t start_memory = { OP_STA_INY, 0x10 }; // sta ($10),Y
  start_memory[0x10] = 0x00;
  start_memory[0x11] = 0x40;

  mem_t end_memory = start_memory;
  end_memory[0x4005] = 0x2a;

  Registers start_registers = { .A = 0x2a, .Y = 0x05 };
  Registers end_registers = { .A = 0x2a, .Y = 0x05, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
static bool test_sta_zpx_wraparound(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_STA_ZPX, 0xf0 };
  mem_t end_memory = start_memory;
  end_memory[0x00] = 0x2a;

  Registers start_registers = { .A = 0x2a, .X = 0x10 };
  Registers end_registers = { .A = 0x2a, .X = 0x10, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sta_inx_wraparound(std::string name) {
  uint64_t cycles = 6;

  mem_t start_memory = { OP_STA_INX, 0xff };
  start_memory[0x03] = 0x00;
  start_memory[0x04] = 0x50;

  mem_t end_memory = start_memory;
  end_memory[0x5000] = 0x2a;

  Registers start_registers = { .A = 0x2a, .X = 0x04 };
  Registers end_registers = { .A = 0x2a, .X = 0x04, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sta_iny_page_cross(std::string name) {
  uint64_t cycles = 6;

  mem_t start_memory = { OP_STA_INY, 0x10 }; // sta ($10),Y
  start_memory[0x10] = 0xFF;
  start_memory[0x11] = 0x30;

  mem_t end_memory = start_memory;
  end_memory[0x31FE] = 0x2a;

  Registers start_registers = { .A = 0x2a, .Y = 0xFF };
  Registers end_registers = { .A = 0x2a, .Y = 0xFF, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
