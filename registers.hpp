#include "types.hpp"

struct Registers {
  mem_addr_t pc = 0;
  mem_data_t A = 0;
  mem_data_t X = 0;
  mem_data_t Y = 0;

  bool operator==(const Registers& other) const {
  return pc == other.pc
         && A == other.A
         && X == other.X
         && Y == other.Y;
  }

  bool operator!=(const Registers& other) const {
    return !(*this == other);
  }

  friend std::ostream& operator<<(std::ostream& o, const Registers& reg) {
    o << std::showbase << std::hex
      << "pc=" << (int)reg.pc
      << ", A=" << (int)reg.A
      << ", X=" << (int)reg.X
      << ", Y=" << (int)reg.Y;
    return o;
  }
};
