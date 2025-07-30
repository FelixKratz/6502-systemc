#include <systemc.h>
#include "memory.hpp"
#include "cpu.hpp"

constexpr uint64_t time_per_cycle = 1000; // Measures in ns -> 1MHz

class Simulation {
  private:
  sc_signal<bool> write_flag, req;
  sc_signal<mem_addr_t> address;
  sc_signal<mem_data_t> write_data, read_data;

  sc_clock clock;

  void connect() {
    cpu.in.clock(clock);
    cpu.in.read_data(read_data);

    cpu.out.req(req);
    cpu.out.address(address);
    cpu.out.write_flag(write_flag);
    cpu.out.write_data(write_data);

    memory.in.clock(clock);
    memory.in.write_flag(write_flag);
    memory.in.address(address);
    memory.in.write_data(write_data);
    memory.in.req(req);

    memory.out.read_data(read_data);

    req.write(false);
  }

  public:
  CPU cpu;
  Memory memory;

  void step(uint64_t count) {
    sc_start(static_cast<double>(time_per_cycle * count), SC_NS);
  }

  void load_program_from_disk(std::string filename, std::optional<mem_addr_t> reset_vector = std::nullopt) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::logic_error("The program file does not exist");

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > 0x10000) {
      throw std::out_of_range("The program does not fit into memory");
    }

    mem_t mem;
    file.read(reinterpret_cast<char*>(&mem[0]), size);
    if (!file.good()) throw std::logic_error("The program file is corrupted");

    if (reset_vector) {
      mem[0xFFFC] = reset_vector.value() & 0xff;
      mem[0xFFFD] = (reset_vector.value() >> 8) & 0xff;
    }

    memory.set_memory(std::move(mem));
  }

  Simulation(bool logging = false) : clock("clock", time_per_cycle, SC_NS), cpu("cpu"), memory("memory") {
    connect();
    cpu.set_logging(logging);
  }
};
