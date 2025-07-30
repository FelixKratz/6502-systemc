#include "simulation.hpp"

int sc_main(int argc, char* argv[]) {
  Simulation simulation(false);
  simulation.load_program_from_disk("bin/6502_functional_test.bin", 0x0400);
  auto start = std::chrono::high_resolution_clock::now();
  simulation.step(100000000);
  auto stop = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = stop - start;
  std::cout << "Execution took " << elapsed.count() << " seconds (real time)" << std::endl;

  uint16_t pc = simulation.cpu.copy_registers().pc;
  if (pc >= 0x3469 && pc <= 0x346c) {
    std::cout << "All tests passed" << std::endl;
  } else {
    std::cout << "Error at test: " << std::hex << std::showbase
              << simulation.cpu.copy_registers().pc << std::endl;
  }
  return 0;
}
