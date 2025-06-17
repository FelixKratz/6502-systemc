#include "simulation.hpp"

int sc_main(int argc, char* argv[]) {
  Simulation simulation(true);

  mem_addr_t counter = 0;
  mem_t memory = {
    OP_LDA_IMM, 0x2,
    OP_STA_ZPG, 0xf0,
    OP_NOP,
    OP_LDA_ZPG, 0xf0,
    OP_ADC_IMM, 0x3,
    OP_STA_ZPG, 0xe0,
    OP_JMP_ABS, 0x11, 0x0,
    OP_NOP,
    OP_NOP,
    OP_NOP,
    OP_NOP,
    OP_BRK,
  };

  simulation.memory.set_memory(std::move(memory));
  simulation.step(100);
  return 0;
}
