#include "simulation.hpp"
#include <cstdint>
#include <fstream>
#include <stdexcept>

mem_t load_program_from_disk(std::string filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) throw std::logic_error("The program file does not exist");

  file.seekg(0, std::ios::end);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  if (size > 0x10000) {
    throw std::out_of_range("The program does not fit into memory");
  }

  mem_t memory;
  file.read(reinterpret_cast<char*>(&memory[0]), size);
  if (!file.good()) throw std::logic_error("The program file is corrupted");
  return memory;
}

int sc_main(int argc, char* argv[]) {
  Simulation simulation(true);

  // Load a simple program into memory
  mem_t memory = {
    OP_LDA_IMM, 0x2,
    OP_STA_ZPG, 0xf0,
    OP_NOP_IMP,
    OP_LDA_ZPG, 0xf0,
    OP_ADC_IMM, 0x3,
    OP_STA_ZPG, 0xe0,
    OP_JMP_ABS, 0x11, 0x0,
    OP_NOP_IMP,
    OP_NOP_IMP,
    OP_NOP_IMP,
    OP_NOP_IMP,
    OP_BRK_IMP,
  };

  mem_t bin = load_program_from_disk("program.bin");

  bin[0xFFFC] = 0x00;
  bin[0xFFFD] = 0x04;

  simulation.memory.set_memory(std::move(bin));

  

  simulation.step(1e9);
  return 0;
}
