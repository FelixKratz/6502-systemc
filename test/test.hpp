#pragma once
#include <systemc>
#include "simulation.hpp"

using TestFunction = bool (*)(std::string name);

struct TestCase {
  std::string name;
  TestFunction function;
};

static bool run_test(std::string&& name,
              uint64_t expected_cycles,
              mem_t& start_memory,
              mem_t& expected_memory,
              Registers& start_registers,
              Registers& expected_registers) {

  bool success = true;
  Simulation simulation;

  simulation.cpu.set_registers(start_registers);
  simulation.memory.set_memory(std::move(start_memory));
  simulation.cpu.set_logging(true);
  simulation.step(1);
  uint64_t start_cycles = simulation.cpu.get_cycle_count();

  while (simulation.cpu.get_cycle_count() < start_cycles + expected_cycles
         && !simulation.cpu.is_halted()) {
    simulation.step(1);
  }
  uint64_t end_cycles = simulation.cpu.get_cycle_count();

  Registers actual_registers = simulation.cpu.copy_registers();
  mem_t actual_memory = simulation.memory.copy_memory();
  uint64_t actual_cycles = end_cycles - start_cycles;

  if (expected_registers != actual_registers) {
    std::cout << std::endl << name << ": register deviation!" << std::endl;
    success = false;
    std::cout << "expected: " << expected_registers << std::endl;
    std::cout << "actual  : " << actual_registers << std::endl;
  }

  if (expected_memory != actual_memory) {
    std::cout << name << ": memory deviation!" << std::endl;
    success = false;
    assert(expected_memory.size() == actual_memory.size());

    std::cout << std::endl << name << " mem diff: [<addr>]=<actual>(<expected>)"
              << std::endl;
    uint32_t diff_limit = 10;
    for (size_t i = 0; i < expected_memory.size(); i++) {
      if (expected_memory[i] != actual_memory[i]) {
        std::cout << std::hex << std::showbase
                  << "[" << i << "]=" << (int)actual_memory[i]
                  << "(" << (int)expected_memory[i] << ")" << std::endl;
        diff_limit--;
      }
      if (diff_limit == 0) {
        std::cout << "..." << std::endl;
        break;
      }
    }
  }

  if (expected_cycles != actual_cycles) {
    std::cout << std::endl << name << ": cycle count deviation!" << std::endl << "Actual: "
              << actual_cycles << " expected: " << expected_cycles << std::endl;
    success = false;
  }

  if (success) {
    std::cout << "[+] " << name << " passed." << std::endl;
  } else {
    std::cout << "[!] " << name << " failed." << std::endl;
  }

  return success;
}
