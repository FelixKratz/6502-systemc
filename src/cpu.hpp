#include <systemc.h>
#include "registers.hpp"
#include "instruction.hpp"

class CPU : public sc_module {
  public:
  struct {
    sc_in<bool> clock;
    sc_in<mem_data_t> read_data;
  } in;

  struct {
    sc_out<bool> req;
    sc_out<bool> write_flag;
    sc_out<mem_addr_t> address;
    sc_out<mem_data_t> write_data;
  } out;

  CPU(sc_module_name name) : sc_module(name) {
    SC_THREAD(execute);
    sensitive << in.clock.pos();

    for (const auto& instruction_group : instruction_set) {
      for (const auto& [opcode, _] : instruction_group.instructions) {
        opcode_map[opcode] = &instruction_group;
      }
    }
  }

  void set_logging(bool log) { logging = log; };
  void set_registers(Registers& reg) { registers = reg; };

  bool is_halted() const { return halted; };
  Registers copy_registers() const { return registers; };
  uint64_t get_cycle_count() const { return cycle_count; };

  private:
  Registers registers;
  bool halted = false;
  bool logging = false;
  uint64_t cycle_count = 0;

  std::unordered_map<opcode_t, const InstructionGroup*> opcode_map;

  // Just a wrapper around wait to count cpu cycles
  void wait() {
    ++cycle_count;
    ::wait();
  }

  // A full handshake transaction with the memory
  mem_data_t memory_transaction() {
    out.req = true;

    // Wait for memory to handle the request.
    // On the 6502 this is guaranteed to happen in 1 cycle.
    wait();
    mem_data_t data = in.read_data;
    out.req = false;
    return data;
  }

  // Read one byte of memory and dont progress the pc
  mem_data_t read_from_memory(const mem_addr_t source_address) {
    out.address = source_address;
    out.write_flag = false;
    return memory_transaction();
  }

  // Write one byte of memory
  mem_data_t write_to_memory(const mem_addr_t address, const mem_data_t data) {
    out.address = address;
    out.write_data = data;
    out.write_flag = true;
    if (logging) std::cout << " -> [" << (int)address << "] = " << (int)data;
    return memory_transaction();
  }

  // Fetch a byte segment from memory and increment the pc
  template <typename T>
  T fetch() {
    static_assert(std::is_trivially_copyable_v<T>,
                  "Template type passed to fetch must be trivially copyable.");

    // Little-endian byte loading
    mem_data_t buffer[sizeof(T)];
    for (size_t i = 0; i < sizeof(T); i++) {
      buffer[i] = read_from_memory(registers.pc++);
    }
    T result;
    std::memcpy(&result, buffer, sizeof(T));
    return result;
  }

  mem_addr_t bus_add_offset(const mem_addr_t base, const int16_t offset, const bool read_mode, const bool penalty_cycle) {
    mem_addr_t result = base + offset;

    // Page-cross cycle
    if (read_mode && ((base & 0xff00) != (result & 0xff00))) wait();
    if (penalty_cycle) wait();

    return result;
  }

  mem_addr_zp_t bus_add_zp_offset(const mem_addr_zp_t base, const mem_addr_zp_t offset) {
    wait();
    return static_cast<mem_addr_zp_t>(base + offset);
  }

  mem_addr_t resolve_indirection(const mem_addr_t iaddr) {
    mem_addr_t lo_byte_addr = iaddr;
    mem_addr_t hi_byte_addr = (iaddr & 0xFF00)
                              | static_cast<mem_addr_zp_t>(iaddr + 1);

    mem_data_t lo_byte = read_from_memory(lo_byte_addr);
    mem_data_t hi_byte = read_from_memory(hi_byte_addr);
    mem_addr_t eaddr = static_cast<mem_addr_t>(lo_byte)
                       | (static_cast<mem_addr_t>(hi_byte) << 8);
    return eaddr;
  }

  mem_addr_t fetch_address(const AddressingMode mode, const bool read_mode) {
    // All address manipulations use a bus cycle.
    mem_addr_t result;
    switch(mode) {

      case AddressingMode::ZeroPage: {
        result = static_cast<mem_addr_t>(fetch<mem_addr_zp_t>());
        break;
      }

      case AddressingMode::ZeroPageX: {
        mem_addr_zp_t base = fetch<mem_addr_zp_t>();
        mem_addr_zp_t offset = static_cast<mem_addr_zp_t>(registers.X);
        result = static_cast<mem_addr_t>(bus_add_zp_offset(base, offset));
        break;
      }

      case AddressingMode::ZeroPageY: {
        mem_addr_zp_t base = fetch<mem_addr_zp_t>();
        mem_addr_zp_t offset = static_cast<mem_addr_zp_t>(registers.Y);
        result = static_cast<mem_addr_t>(bus_add_zp_offset(base, offset));
        break;
      }

      case AddressingMode::Absolute: {
        result = fetch<mem_addr_t>();
        break;
      }

      case AddressingMode::AbsoluteX: {
        mem_addr_t base = fetch<mem_addr_t>();
        mem_addr_t offset = static_cast<mem_addr_t>(registers.X);
        result = bus_add_offset(base, offset, read_mode, !read_mode);
        break;
      }

      case AddressingMode::AbsoluteY: {
        mem_addr_t base = fetch<mem_addr_t>();
        mem_addr_t offset = static_cast<mem_addr_t>(registers.Y);
        result = bus_add_offset(base, offset, read_mode, !read_mode);
        break;
      }

      case AddressingMode::Indirect: {
        mem_addr_t iaddr = fetch<mem_addr_t>();
        result = resolve_indirection(iaddr);
        break;
      }

      case AddressingMode::IndirectX: {
        mem_addr_zp_t base = fetch<mem_addr_zp_t>();
        mem_addr_zp_t offset = static_cast<mem_addr_zp_t>(registers.X);
        mem_addr_zp_t iaddr = bus_add_zp_offset(base, offset);
        result = resolve_indirection(iaddr);
        break;
      }

      case AddressingMode::IndirectY: {
        mem_addr_zp_t iaddr = fetch<mem_addr_zp_t>();
        mem_addr_t base = resolve_indirection(iaddr);
        mem_addr_t offset = registers.Y;
        result = bus_add_offset(base, offset, read_mode, !read_mode);
        break;
      }

      case AddressingMode::Relative: {
        int8_t offset = static_cast<int8_t>(fetch<mem_data_t>());
        result = bus_add_offset(registers.pc, offset, true, true);
        break;
      }

      default:
        throw std::logic_error("Invalid addressing mode!");
    }

    if (logging) std::cout << " $" << (int)result;
    return result;
  }

  mem_data_t resolve_operand_data(const AddressingMode mode) {
    if (mode == AddressingMode::Immediate) {
      mem_data_t result = fetch<mem_data_t>();
      if (logging) std::cout << " #" << (int)result;
      return result;
    }

    mem_addr_t address = fetch_address(mode, true);
    mem_data_t result = read_from_memory(address);

    if (logging) std::cout << " -> " << (int)result;
    return result;
  }

  // Core loop of the CPU
  void execute() {
    while (!halted) {
      wait();
      uint64_t cycles_start = cycle_count;

      opcode_t opcode = fetch<opcode_t>();
      auto function_map_it = opcode_map.find(opcode);

      if (function_map_it != opcode_map.end()) {
        if (logging) {
          std::cout << std::endl << std::setw(8) << sc_time_stamp() << ": "
                    << std::showbase << std::hex
                    << function_map_it->second->name;
        }

        const InstructionGroup* group = function_map_it->second;
        AddressingMode mode = group->get_mode(opcode);
        InstructionHandler handler = function_map_it->second->handler;

        // Call the OPCode handler function
        (this->*handler)(mode);

        if (logging) {
          uint64_t cycles_end = cycle_count;
          std::cout << std::noshowbase << std::dec
                    << " (" << cycles_end - cycles_start << " cycles)"
                    << std::endl;
        }
      }
      else {
          std::cout << sc_time_stamp() << ": Unknown opcode "
                    << (int)opcode << std::endl;
          halted = true;
      }
    };
  }

  /* Implementation of the instruction set */
  const std::vector<const InstructionGroup> instruction_set = {
    { "brk", &CPU::brk, {
        { OP_BRK, Implied },
      }
    },
    { "jmp", &CPU::jmp, {
        { OP_JMP_ABS, Absolute },
        { OP_JMP_IND, Indirect },
      }
    },
    { "sta", &CPU::sta, {
        { OP_STA_ZPG, ZeroPage  },
        { OP_STA_ZPX, ZeroPageX },
        { OP_STA_ABS, Absolute  },
        { OP_STA_ABX, AbsoluteX },
        { OP_STA_ABY, AbsoluteY },
        { OP_STA_INX, IndirectX },
        { OP_STA_INY, IndirectY },
      }
    },
    { "stx", &CPU::stx, {
        { OP_STX_ZPG, ZeroPage  },
        { OP_STX_ABS, Absolute  },
        { OP_STX_ZPY, ZeroPageY },
      }
    },
    { "sty", &CPU::sty, {
        { OP_STY_ZPG, ZeroPage  },
        { OP_STY_ABS, Absolute  },
        { OP_STY_ZPX, ZeroPageX },
      }
    },
    { "lda", &CPU::lda, {
        { OP_LDA_IMM, Immediate },
        { OP_LDA_ZPG, ZeroPage  },
        { OP_LDA_ZPX, ZeroPageX },
        { OP_LDA_ABS, Absolute  },
        { OP_LDA_ABX, AbsoluteX },
        { OP_LDA_ABY, AbsoluteY },
        { OP_LDA_INX, IndirectX },
        { OP_LDA_INY, IndirectY },
      }
    },
    { "ldx", &CPU::ldx, {
        { OP_LDX_IMM, Immediate },
        { OP_LDX_ZPG, ZeroPage  },
        { OP_LDX_ZPY, ZeroPageY  },
        { OP_LDX_ABS, Absolute  },
        { OP_LDX_ABY, AbsoluteY },
      }
    },
    { "ldy", &CPU::ldy, {
        { OP_LDY_IMM, Immediate },
        { OP_LDY_ZPG, ZeroPage  },
        { OP_LDY_ZPX, ZeroPageX  },
        { OP_LDY_ABS, Absolute  },
        { OP_LDY_ABX, AbsoluteX },
      }
    },
    { "adc", &CPU::adc, {
        { OP_ADC_IMM, Immediate },
        { OP_ADC_ZPG, ZeroPage  },
        { OP_ADC_ABS, Absolute  },
        { OP_ADC_ABX, AbsoluteX },
        { OP_ADC_ABY, AbsoluteY },
        { OP_ADC_ZPX, ZeroPageX },
        { OP_ADC_INX, IndirectX },
        { OP_ADC_INY, IndirectY },
      }
    },
    { "tax", &CPU::tax, {
        { OP_TAX_IMP, Implied },
      }
    },
    { "bcc", &CPU::bcc, {
        { OP_BCC_REL, Relative },
      }
    },
    { "nop", &CPU::nop, {
        { OP_NOP, Immediate },
      }
    },
  };

  void adc(const AddressingMode mode) {
    mem_data_t M = resolve_operand_data(mode);
    uint16_t sum = static_cast<uint16_t>(registers.A) + M + registers.P.C;

    registers.P.C = sum > 0xff;
    registers.P.Z = (sum & 0xff) == 0;
    registers.P.N = (sum & 0x80) != 0;
    registers.P.V = (~(registers.A ^ M) & (registers.A ^ static_cast<uint8_t>(sum)) & 0x80) != 0;

    registers.A = static_cast<mem_data_t>(sum);
  }

  void st_(const AddressingMode mode, const mem_data_t data) {
    mem_addr_t destination = fetch_address(mode, false);
    write_to_memory(destination, data);
  }

  void sta(const AddressingMode mode) {
    st_(mode, registers.A);
  }

  void stx(const AddressingMode mode) {
    st_(mode, registers.X);
  }

  void sty(const AddressingMode mode) {
    st_(mode, registers.Y);
  }

  void ld_(const AddressingMode mode, mem_data_t& destination) {
    destination = resolve_operand_data(mode);
    registers.P.Z = destination == 0;
    registers.P.N = (destination & 0x80) != 0;
  }

  void lda(const AddressingMode mode) {
    ld_(mode, registers.A);
  }

  void ldx(const AddressingMode mode) {
    ld_(mode, registers.X);
  }

  void ldy(const AddressingMode mode) {
    ld_(mode, registers.Y);
  }

  void jmp(const AddressingMode mode) {
    registers.pc = fetch_address(mode, false);
  }

  void brk(const AddressingMode mode) {
    // brk actually takes 7 instructions, which we model later
    for (int i = 0; i < 6; i++) wait();
    registers.P.B = true;
    halted = true;
  }

  void tax(const AddressingMode mode) {
    registers.X = registers.A;
    wait();
    registers.P.Z = registers.A == 0;
    registers.P.N = (registers.A & 0x80) != 0;
  }

  void bcc(const AddressingMode mode) {
    if (!registers.P.C) {
      mem_addr_t new_pc = fetch_address(mode, false);
      registers.pc = new_pc;
    } else {
      fetch<mem_addr_zp_t>();
    }
  }

  void nop(const AddressingMode _) {
    wait();
  }
};
