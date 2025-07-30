#include <systemc.h>
#include "registers.hpp"
#include "instruction.hpp"

template <class Derived>
class CPUCore : public sc_module {
  friend Derived;

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

  CPUCore(sc_module_name name) : sc_module(name) {
    SC_THREAD(execute);
    sensitive << in.clock.pos();
  }

  void set_logging(bool log) { logging = log; };
  void set_registers(Registers& reg) { registers = reg; };

  bool is_booted() const { return booted; }
  bool is_halted() const { return halted; };
  Registers copy_registers() const { return registers; };
  uint64_t get_cycle_count() const { return cycle_count; };

  private:
  Registers registers;
  bool halted = false;
  bool logging = false;
  bool booted = false;
  uint64_t cycle_count = 0;

  std::array<Instruction, 0xff> opcode_map = {};
  std::array<std::string, 0xff> opcode_names = {};

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
      buffer[i] = read_from_memory(registers.pc);
      registers.pc++;
    }
    T result;
    std::memcpy(&result, buffer, sizeof(T));
    return result;
  }

  mem_addr_t bus_add_offset(const mem_addr_t base, const int16_t offset, const MemoryAccessType mat) {
    mem_addr_t result = base + offset;

    bool page_cross = (base & 0xff00) != (result & 0xff00);
    if (mat == Read && page_cross) wait(); // Page-cross cycle
    if (mat == Write) wait(); // Hardware penalty cycle
    if (mat == ReadModifyWrite) read_from_memory(result); // Dummy read

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

  mem_addr_t fetch_address(const AddressingMode mode, const MemoryAccessType mat) {
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
        result = bus_add_offset(base, offset, mat);
        break;
      }

      case AddressingMode::AbsoluteY: {
        mem_addr_t base = fetch<mem_addr_t>();
        mem_addr_t offset = static_cast<mem_addr_t>(registers.Y);
        result = bus_add_offset(base, offset, mat);
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
        result = bus_add_offset(base, offset, mat);
        break;
      }

      case AddressingMode::Relative: {
        int8_t offset = static_cast<int8_t>(fetch<mem_data_t>());
        result = bus_add_offset(registers.pc, static_cast<int16_t>(offset), mat);
        break;
      }

      default:
        throw std::logic_error("Invalid addressing mode!");
    }

    if (logging) std::cout << " $" << (int)result;
    return result;
  }

  Operand resolve_operand(const AddressingMode mode, MemoryAccessType mat) {
    // Protect against non-argument operands (IMP)
    assert(mode != AddressingMode::Implied);

    // Resolve non-memory operands (IMM,ACC)
    if (mode == AddressingMode::Immediate) {
      mem_data_t result = fetch<mem_data_t>();
      if (logging) std::cout << " #" << (int)result;
      return { result, mode };
    } else if (mode == AddressingMode::Accumulator) {
      mem_data_t result = registers.A;
      if (logging) std::cout << " A=(" << (int)result << ")";
      return { result, mode, [&](mem_data_t value){registers.A = value;} };
    }

    // Resolve memory operands (ZPG,ZPX,ZPY,ABS,ABX,ABY,IND,INX,INY,REL)
    mem_addr_t address = fetch_address(mode, mat);
    mem_data_t result = read_from_memory(address);

    if (logging) std::cout << " -> " << (int)result;
    return { result, mode, [=](mem_data_t value){write_to_memory(address, value);} };
  }

  template <typename T>
  void push_stack(T data) {
    static_assert(std::is_trivially_copyable_v<T>,
                  "Template type passed to push_stack must be trivially copyable.");

    mem_data_t buffer[sizeof(T)];
    std::memcpy(buffer, &data, sizeof(T));
    for (int i = sizeof(T) - 1; i >= 0; i--) {
      write_to_memory(0x0100 + registers.S, buffer[i]);
      registers.S--;
    }
    wait();
  }

  template <typename T>
  T pull_stack() {
    static_assert(std::is_trivially_copyable_v<T>,
                  "Template type passed to pull_stack must be trivially copyable.");

    read_from_memory(0x0100 + registers.S + 1); // Dummy read

    // Little-endian byte loading
    mem_data_t buffer[sizeof(T)];
    for (size_t i = 0; i < sizeof(T); i++) {
      buffer[i] = read_from_memory(0x0100 + ++registers.S);
    }
    wait();
    T result;
    std::memcpy(&result, buffer, sizeof(T));
    return result;
  }

  void branch(const AddressingMode mode, const bool condition) {
    if (condition) {
      mem_addr_t new_pc = fetch_address(mode, Read);
      wait();
      registers.pc = new_pc;
    } else { fetch<mem_addr_zp_t>(); } // Fetch the offset byte anyways
  }

  void store(const AddressingMode mode, const mem_data_t data) {
    mem_addr_t destination = fetch_address(mode, Write);
    write_to_memory(destination, data);
  }

  void load(const AddressingMode mode, mem_data_t& destination) {
    destination = resolve_operand(mode, Read).data;
    registers.P.update_nz(destination);
  }

  void transfer(const mem_data_t from, mem_data_t& to) {
    to = from;
    wait();
    registers.P.update_nz(from);
  }

  void compare(const AddressingMode mode, mem_data_t a) {
    mem_data_t M = resolve_operand(mode, Read).data;
    mem_data_t result = a - M;

    registers.P.update_nz(result);
    registers.P.C = a >= M;
  }

  // Used for all RMW operations to write back the result
  void write_result(Operand operand, mem_data_t result) {
    if (operand.mode == Accumulator) wait(); // ALU cylce
    else operand.write_back(operand.data); // Dummy write

    registers.P.update_nz(result);
    operand.write_back(result);
  }

  void clear_flag(flag_t& flag) {
    flag = 0;
    wait();
  }

  void set_flag(flag_t& flag) {
    flag = 1;
    wait();
  }

  void offset_register(mem_data_t& reg, int8_t offset) {
    reg += offset;
    wait();
    registers.P.update_nz(reg);
  }

  void boot() {
    wait();
    if (registers.pc == 0) {
      mem_data_t pcl = read_from_memory(0xFFFC);
      mem_data_t pch = read_from_memory(0xFFFD);
      mem_addr_t pc = pcl | (static_cast<mem_addr_t>(pch) << 8);
      registers.pc = pc;
    }
    booted = true;
  }

  // Core loop of the CPU
  void execute() {
    boot();

    while (!halted) {
      const uint64_t cycles_start = cycle_count;

      const opcode_t opcode = fetch<opcode_t>();
      const Instruction instruction = opcode_map[opcode];

      if (instruction.handler != nullptr) {
        if (logging) {
          std::cout << std::endl << std::setw(8) << sc_time_stamp() << ": "
                    << std::showbase << std::hex
                    << opcode_names[opcode];
        }

        AddressingMode mode = instruction.mode;
        InstructionHandler handler = instruction.handler;

        // Call the OPCode handler function
        (reinterpret_cast<Derived*>(this)->*handler)(mode);

        if (logging) {
          const uint64_t cycles_end = cycle_count;
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
};
