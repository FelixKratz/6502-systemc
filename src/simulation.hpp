#include "memory.hpp"
#include "cpu.hpp"
#include "driver.hpp"

class Simulation {
  private:
  Bus bus;
  SystemCSimulationDriver<CPU> cpu_driver;

  public:
  CPU cpu;
  Memory memory;

  void step(uint64_t count) { cpu_driver.step(count); }

  Simulation(bool logging = false) : cpu_driver(&cpu, &CPU::execute, std::string("CPUDriver")), cpu(&cpu_driver, &bus) {
    cpu.set_logging(logging);
    bus.attach(&memory);
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

