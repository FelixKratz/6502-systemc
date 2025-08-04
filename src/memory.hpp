#include "types.hpp"
#include "bus.hpp"

class Memory : public BusDevice {
  public:
  void set_memory(mem_t&& mem) { memory = std::move(mem); }
  mem_t copy_memory() { return memory; }

  Memory() { memory.fill(0); }

  private:
  mem_t memory;

  virtual mem_data_t read(const mem_addr_t address) const override {
    return memory[address];
  }

  virtual void write(const mem_addr_t address, const mem_data_t data) override {
    memory[address] = data;
  }
};
