#include "simulation.hpp"

int sc_main(int argc, char* argv[]) {
  Simulation simulation(false);
  simulation.set_mode(SimulationMode::LogicAccurate);
  simulation.load_program_from_disk("bin/6502_functional_test.bin", 0x0400);

  sc_core::sc_time sc_start = sc_core::sc_time_stamp();
  auto start = std::chrono::high_resolution_clock::now();
  simulation.step(100000000);
  auto stop = std::chrono::high_resolution_clock::now();
  sc_core::sc_time sc_end = sc_core::sc_time_stamp();

  std::chrono::duration<double> elapsed = stop - start;
  std::cout << "Execution took " << elapsed.count() << " seconds (real time), "
            << sc_end - sc_start << " (simulation time) and " << simulation.cpu.get_cycle_count() << " cycles" << std::endl;

  uint16_t pc = simulation.cpu.copy_registers().pc;
  if (pc >= 0x3469 && pc <= 0x346c) {
    std::cout << "All tests passed" << std::endl;
  } else {
    std::cout << "Error at test: " << std::hex << std::showbase
              << simulation.cpu.copy_registers().pc << std::endl;
  }
  return 0;
}
