#include <systemc.h>
#include "memory.hpp"
#include "cpu.hpp"

constexpr uint64_t time_per_cycle = 1000; // Measures in ns -> 1MHz

class Simulation {
  private:
  Bus bus;
  sc_core::sc_clock clock;

  public:
  CPU cpu;
  Memory memory;

  void step(uint64_t count) {
    if (cpu.get_accuracy() != TimingAccurate) { sc_start(1, SC_NS); }
    else sc_start(static_cast<double>(time_per_cycle * count), SC_NS);
  }

  void load_program_from_disk(std::string filename, std::optional<mem_addr_t> reset_vector = std::nullopt) {
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

  Simulation(bool logging = false) : clock("clock", time_per_cycle, SC_NS), cpu("cpu", clock.posedge_event(), &bus) {
    cpu.set_logging(logging);
    bus.attach(&memory);
  }
};
