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

  mem_addr_zp_t bus_add_offset(const mem_addr_zp_t base, const mem_addr_zp_t offset) {
    wait();
    return base + offset;
  }

  mem_addr_t fetch_address(const AddressingMode mode) {
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
        result = static_cast<mem_addr_t>(bus_add_offset(base, offset));
        break;
      }

      case AddressingMode::Absolute: {
        result = fetch<mem_addr_t>();
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

    mem_addr_t address = fetch_address(mode);
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
      }
    },
    { "sta", &CPU::sta, {
        { OP_STA_ZPG, ZeroPage },
        { OP_STA_ZPX, ZeroPageX },
      }
    },
    { "lda", &CPU::lda, {
        { OP_LDA_ZPG, ZeroPage },
        { OP_LDA_ZPX, ZeroPageX },
        { OP_LDA_IMM, Immediate },
      }
    },
    { "adc", &CPU::adc, {
        { OP_ADC_IMM, Immediate },
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

  void sta(const AddressingMode mode) {
    mem_addr_t destination = fetch_address(mode);
    write_to_memory(destination, registers.A);
  }

  void lda(const AddressingMode mode) {
    registers.A = resolve_operand_data(mode);
    registers.P.Z = registers.A == 0;
    registers.P.N = (registers.A & 0x80) != 0;
  }

  void jmp(const AddressingMode mode) {
    registers.pc = fetch_address(mode);
  }

  void brk(const AddressingMode mode) {
    // brk actually takes 7 instructions, which we model later
    for (int i = 0; i < 6; i++) wait();
    registers.P.B = true;
    halted = true;
  }

  void nop(const AddressingMode _) {
    wait();
  }
};
