#include "simulation.hpp"

int sc_main(int argc, char* argv[]) {
  Simulation simulation(true);

  // Load a simple program into memory
  mem_t memory = {
    OP_LDA_IMM, 0x2,
    OP_STA_ZPG, 0xf0,
    OP_NOP_IMM,
    OP_LDA_ZPG, 0xf0,
    OP_ADC_IMM, 0x3,
    OP_STA_ZPG, 0xe0,
    OP_JMP_ABS, 0x11, 0x0,
    OP_NOP_IMM,
    OP_NOP_IMM,
    OP_NOP_IMM,
    OP_NOP_IMM,
    OP_BRK_IMP,
  };

  simulation.memory.set_memory(std::move(memory));
  simulation.step(100);
  return 0;
}
