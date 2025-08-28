#include "memory.hpp"
#include "cpu.hpp"
#include "driver.hpp"
#include <exception>
#include <stdexcept>

enum SimulationMode {
  TimingAccurate,
  LogicAccurate,
};

class Simulation {
  private:
  Bus bus;
  SystemCSimulationDriver<CPU> cpu_driver;
  ManualSimulationDriver<CPU> manual_driver;
  SimulationMode mode = SimulationMode::TimingAccurate;

  public:
  CPU cpu;
  Memory memory;

  void step(uint64_t count) {
    if (mode == SimulationMode::TimingAccurate) {
      cpu_driver.step(count);
    } else if (mode == SimulationMode::LogicAccurate) {
      manual_driver.step(count);
    } else {
      throw std::logic_error("Invalid SimulationMode.");
    }
  }

  Simulation(bool logging = false) : cpu_driver(&cpu, &CPU::execute, std::string("CPUDriver")), cpu(&cpu_driver, &bus), manual_driver(&cpu, &CPU::execute, std::string("ManualDriver")) {
    cpu.set_logging(logging);
    bus.attach(&memory);
  }

  void set_mode(SimulationMode simulation_mode) {
    mode = simulation_mode;
    if (mode == TimingAccurate) cpu.set_driver(&cpu_driver);
    else if (mode == LogicAccurate) cpu.set_driver(&manual_driver);
  }
  void load_program_from_disk(std::string filename, std::optional<mem_addr_t> reset_vector = std::nullopt);
};


inline void Simulation::load_program_from_disk(std::string filename, std::optional<mem_addr_t> reset_vector) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) throw std::logic_error("The program file does not exist");

  file.seekg(0, std::ios::end);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  mem_t mem;
  if (size > mem.size()) {
    throw std::out_of_range("The program does not fit into memory");
  }

  file.read(reinterpret_cast<char*>(&mem[0]), size);
  if (!file.good()) throw std::logic_error("The program file is corrupted");

  if (reset_vector) {
    mem[0xFFFC] = reset_vector.value() & 0xff;
    mem[0xFFFD] = (reset_vector.value() >> 8) & 0xff;
  }

  memory.set_memory(std::move(mem));
}

