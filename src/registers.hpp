#include "types.hpp"
#include <ios>

constexpr uint8_t FLAG_C = 1 << 0;  // Carry
constexpr uint8_t FLAG_Z = 1 << 1;  // Zero
constexpr uint8_t FLAG_I = 1 << 2;  // Interrupt disable
constexpr uint8_t FLAG_D = 1 << 3;  // Decimal mode
constexpr uint8_t FLAG_B = 1 << 4;  // Break
constexpr uint8_t FLAG_U = 1 << 5;  // Unused
constexpr uint8_t FLAG_V = 1 << 6;  // Overflow
constexpr uint8_t FLAG_N = 1 << 7;  // Negative

struct StatusRegister {
  bool C = false;
  bool Z = false;
  bool I = false;
  bool D = false;
  bool B = false;
  bool U = false;
  bool V = false;
  bool N = false;

  uint8_t to_byte() {
    return (N*FLAG_N) | (V*FLAG_V) | (U*FLAG_U) | (B*FLAG_B)
           | (D*FLAG_D) | (I*FLAG_I) | (Z*FLAG_Z) | (C*FLAG_C);
  }

  void from_byte(uint8_t byte) {
    C = byte & FLAG_C;
    Z = byte & FLAG_Z;
    I = byte & FLAG_I;
    D = byte & FLAG_D;
    B = byte & FLAG_B;
    U = byte & FLAG_U;
    V = byte & FLAG_V;
    N = byte & FLAG_N;
  }

  void set_carry(const uint16_t number) { C = number > 0xff; }
  void set_zero(const mem_data_t number) { Z = number == 0; }
  void set_negative(const mem_data_t number) { N = (number & 0x80) != 0; }
  void set_overflow(const mem_data_t arg1, const mem_data_t arg2, const mem_data_t result) { V = (~(arg2 ^ arg1) & (arg2 ^ result) & 0x80) != 0; }

  bool operator==(const StatusRegister& other) const {
    return C == other.C && Z == other.Z && I == other.I && D == other.D
           && B == other.B && U == other.U && V == other.V && N == other.N;
  }

  friend std::ostream& operator<<(std::ostream& o, const StatusRegister& p) {
    o << std::noshowbase << "NV-BDIZC: "
      << p.N << p.V << p.U << p.B << p.D << p.I << p.Z << p.C;
    return o;
  }
};

struct Registers {
  mem_addr_t pc = 0;
  mem_data_t A = 0;
  mem_data_t X = 0;
  mem_data_t Y = 0;
  StatusRegister P;

  bool operator==(const Registers& other) const {
  return pc == other.pc && A == other.A && X == other.X && Y == other.Y
         && P == other.P;
  }

  bool operator!=(const Registers& other) const {
    return !(*this == other);
  }

  friend std::ostream& operator<<(std::ostream& o, const Registers& reg) {
    o << std::showbase << std::hex
      << "pc=" << (int)reg.pc
      << ", A=" << (int)reg.A
      << ", X=" << (int)reg.X
      << ", Y=" << (int)reg.Y
      << ", P=" << reg.P;
    return o;
  }
};
