#include <systemc.h>
#include "types.hpp"

class Memory : public sc_module {
  public:
  struct Input {
    sc_core::sc_in<mem_addr_t> address;
    sc_core::sc_in<mem_data_t> write_data;
    sc_core::sc_in<bool> req, clock, write_flag;
  } in;

  struct Output {
    sc_core::sc_out<mem_data_t> read_data;
  } out;

  void set_memory(mem_t&& mem) { memory = std::move(mem); }
  mem_t copy_memory() { return memory; }

  Memory(sc_module_name name) : sc_module(name) {
    memory.fill(0);
    SC_METHOD(operate);
    sensitive << in.clock.neg();
    dont_initialize();
  }

  private:
  mem_t memory;

  void operate() {
    if (in.req) {
      if (in.write_flag) memory[in.address] = in.write_data;
      else out.read_data = memory[in.address];
    }
  }
};
