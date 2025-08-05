#pragma once
#include <vector>
#include "types.hpp"

class BusDevice {
  public:
  BusDevice() {}
  bool addressable(mem_addr_t addr) const { return true; }
  virtual mem_data_t read(const mem_addr_t address) const = 0;
  virtual void write(const mem_addr_t address, const mem_data_t data) = 0;
};

class Bus {
  public:
  void attach(BusDevice* device) {
    devices.push_back(device);
  }

  void write(mem_addr_t address, mem_data_t data) {
    for (auto& device : devices) {
      if (device->addressable(address)) device->write(address, data);
    }
  }

  mem_data_t read(mem_addr_t address) {
    for (auto& device : devices) {
      if (device->addressable(address)) return device->read(address);
    }
    throw std::logic_error("No device attached that could handle the address");
  }

  private:
  std::vector<BusDevice*> devices;
};
