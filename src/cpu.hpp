#include "cpu_core.hpp"

class CPU : public CPUCore<CPU> {
  private:
  /* Implementation of the instruction set */
  void adc(const AddressingMode mode) {
    mem_data_t M = resolve_operand(mode, Read).data;
    uint16_t sum = static_cast<uint16_t>(registers.A) + M + registers.P.C;
    mem_data_t result = static_cast<mem_data_t>(sum);

    registers.P.update_nz(result);
    registers.P.update_carry(sum);
    registers.P.update_overflow(M, registers.A, result);

    registers.A = result;
  }

  void sbc(const AddressingMode mode) {
    mem_data_t M = resolve_operand(mode, Read).data ^ 0xff;
    uint16_t sum = static_cast<uint16_t>(registers.A) + M + registers.P.C;
    mem_data_t result = static_cast<mem_data_t>(sum);

    registers.P.update_nz(result);
    registers.P.update_carry(sum);
    registers.P.update_overflow(M, registers.A, result);

    registers.A = result;
  }

  void asl(const AddressingMode mode) {
    Operand operand = resolve_operand(mode, ReadModifyWrite);
    mem_data_t M = operand.data;
    registers.P.C = (M & 0x80) != 0;
    mem_data_t result = M << 1;

    write_result(operand, result);
  }

  void lsr(const AddressingMode mode) {
    Operand operand = resolve_operand(mode, ReadModifyWrite);
    mem_data_t M = operand.data;
    registers.P.C = M & 0x01;
    mem_data_t result = M >> 1;

    write_result(operand, result);
  }

  void rol(const AddressingMode mode) {
    Operand operand = resolve_operand(mode, ReadModifyWrite);
    mem_data_t M = operand.data;
    flag_t old_carry = registers.P.C;
    registers.P.C = (M & 0x80) != 0;
    mem_data_t result = static_cast<mem_data_t>((M << 1) | old_carry);

    write_result(operand, result);
  }

  void ror(const AddressingMode mode) {
    Operand operand = resolve_operand(mode, ReadModifyWrite);
    mem_data_t M = operand.data;
    flag_t old_carry = registers.P.C;
    registers.P.C = M & 0x01;
    mem_data_t result = static_cast<mem_data_t>((M >> 1) | (0x80 * old_carry));

    write_result(operand, result);
  }

  void inc(const AddressingMode mode) {
    Operand operand = resolve_operand(mode, ReadModifyWrite);
    mem_data_t result = operand.data + 1;

    write_result(operand, result);
  }

  void dec(const AddressingMode mode) {
    Operand operand = resolve_operand(mode, ReadModifyWrite);
    mem_data_t result = operand.data - 1;

    write_result(operand, result);
  }

  void and_instr(const AddressingMode mode) {
    mem_data_t M = resolve_operand(mode, Read).data;
    registers.A &= M;
    registers.P.update_nz(registers.A);
  }

  void eor(const AddressingMode mode) {
    mem_data_t M = resolve_operand(mode, Read).data;
    registers.A ^= M;
    registers.P.update_nz(registers.A);
  }

  void ora(const AddressingMode mode) {
    mem_data_t M = resolve_operand(mode, Read).data;
    registers.A |= M;
    registers.P.update_nz(registers.A);
  }

  void bit(const AddressingMode mode) {
    mem_data_t M = resolve_operand(mode, Read).data;
    mem_data_t result = registers.A & M;
    registers.P.update_zero(result);
    registers.P.update_negative(M);
    registers.P.V = (M & 0x40) != 0;
  }

  void pha(const AddressingMode _) { push_stack(registers.A); }

  void php(const AddressingMode _) {
    StatusRegister p = registers.P;
    p.B = 1; // Break flag always set on php
    p.U = 1; // Unused flag always set on php
    push_stack(p.to_byte());
  }

  void pla(const AddressingMode _) {
    registers.A = pull_stack<mem_data_t>();
    registers.P.update_nz(registers.A);
  }

  void plp(const AddressingMode _) {
    mem_data_t stack_P = pull_stack<mem_data_t>();
    registers.P.from_byte(stack_P);
  }
  
  void jmp(const AddressingMode mode) {
    registers.pc = fetch_address(mode, Read);
  }

  void jsr(const AddressingMode mode) {
    mem_addr_t sr_pc = fetch_address(mode, Read);
    mem_addr_t return_address = registers.pc - 1;

    push_stack(return_address);
    registers.pc = sr_pc;
  }

  void rts(const AddressingMode _) {
    mem_addr_t return_address = pull_stack<mem_addr_t>();
    wait();
    registers.pc = return_address + 1;
  }

  void rti(const AddressingMode _) {
    #pragma pack(push, 1)
    struct rti_data_t { mem_data_t p; mem_addr_t pc; };
    #pragma pack(pop)

    rti_data_t result = pull_stack<rti_data_t>();
    registers.P.from_byte(result.p);
    registers.pc = result.pc;
  }

  void brk(const AddressingMode mode) {
    #pragma pack(push, 1)
    struct brk_data_t { mem_data_t p; mem_data_t pch; mem_data_t pcl; } data;
    #pragma pack(pop)
    data.p = registers.P.to_byte() | 0x30;

    mem_addr_t pc = registers.pc + 1;
    data.pch = static_cast<mem_data_t>(pc >> 8); // High byte first
    data.pcl = static_cast<mem_data_t>(pc & 0xff); // Low byte second

    push_stack(data);

    registers.P.I = 1;

    mem_data_t pcl = read_from_memory(0xFFFE);
    mem_data_t pch = read_from_memory(0xFFFF);
    registers.pc = (pch << 8) | pcl;
  }

  void bcc(const AddressingMode mode) { branch(mode, !registers.P.C); }
  void bcs(const AddressingMode mode) { branch(mode, registers.P.C); }
  void bne(const AddressingMode mode) { branch(mode, !registers.P.Z); }
  void beq(const AddressingMode mode) { branch(mode, registers.P.Z); }
  void bpl(const AddressingMode mode) { branch(mode, !registers.P.N); }
  void bmi(const AddressingMode mode) { branch(mode, registers.P.N); }
  void bvc(const AddressingMode mode) { branch(mode, !registers.P.V); }
  void bvs(const AddressingMode mode) { branch(mode, registers.P.V); }

  void cmp(const AddressingMode mode) { compare(mode, registers.A); }
  void cpx(const AddressingMode mode) { compare(mode, registers.X); }
  void cpy(const AddressingMode mode) { compare(mode, registers.Y); }

  void sta(const AddressingMode mode) { store(mode, registers.A); }
  void stx(const AddressingMode mode) { store(mode, registers.X); }
  void sty(const AddressingMode mode) { store(mode, registers.Y); }

  void lda(const AddressingMode mode) { load(mode, registers.A); }
  void ldx(const AddressingMode mode) { load(mode, registers.X); }
  void ldy(const AddressingMode mode) { load(mode, registers.Y); }

  void tax(const AddressingMode _) { transfer(registers.A, registers.X); }
  void tay(const AddressingMode _) { transfer(registers.A, registers.Y); }
  void txa(const AddressingMode _) { transfer(registers.X, registers.A); }
  void tya(const AddressingMode _) { transfer(registers.Y, registers.A); }
  void tsx(const AddressingMode _) { transfer(registers.S, registers.X); }
  void txs(const AddressingMode _) { registers.S = registers.X; wait(); }

  void clc(const AddressingMode _) { clear_flag(registers.P.C); }
  void cld(const AddressingMode _) { clear_flag(registers.P.D); }
  void cli(const AddressingMode _) { clear_flag(registers.P.I); }
  void clv(const AddressingMode _) { clear_flag(registers.P.V); }

  void sec(const AddressingMode _) { set_flag(registers.P.C); }
  void sed(const AddressingMode _) { set_flag(registers.P.D); }
  void sei(const AddressingMode _) { set_flag(registers.P.I); }

  void inx(const AddressingMode mode) { offset_register(registers.X, 1); }
  void iny(const AddressingMode mode) { offset_register(registers.Y, 1); }
  void dex(const AddressingMode mode) { offset_register(registers.X, -1); }
  void dey(const AddressingMode mode) { offset_register(registers.Y, -1); }

  void nop(const AddressingMode _) { wait(); }

  static const inline std::vector<const InstructionGroup> instruction_set = {
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
    { "sbc", &CPU::sbc, {
        { OP_SBC_IMM, Immediate },
        { OP_SBC_ZPG, ZeroPage  },
        { OP_SBC_ABS, Absolute  },
        { OP_SBC_ABX, AbsoluteX },
        { OP_SBC_ABY, AbsoluteY },
        { OP_SBC_ZPX, ZeroPageX },
        { OP_SBC_INX, IndirectX },
        { OP_SBC_INY, IndirectY },
      }
    },
    { "asl", &CPU::asl, {
        { OP_ASL_ACC, Accumulator },
        { OP_ASL_ZPG, ZeroPage    },
        { OP_ASL_ZPX, ZeroPageX   },
        { OP_ASL_ABS, Absolute    },
        { OP_ASL_ABX, AbsoluteX   },
      }
    },
    { "lsr", &CPU::lsr, {
        { OP_LSR_ACC, Accumulator },
        { OP_LSR_ZPG, ZeroPage    },
        { OP_LSR_ZPX, ZeroPageX   },
        { OP_LSR_ABS, Absolute    },
        { OP_LSR_ABX, AbsoluteX   },
      }
    },
    { "rol", &CPU::rol, {
        { OP_ROL_ACC, Accumulator },
        { OP_ROL_ZPG, ZeroPage    },
        { OP_ROL_ZPX, ZeroPageX   },
        { OP_ROL_ABS, Absolute    },
        { OP_ROL_ABX, AbsoluteX   },
      }
    },
    { "ror", &CPU::ror, {
        { OP_ROR_ACC, Accumulator },
        { OP_ROR_ZPG, ZeroPage    },
        { OP_ROR_ZPX, ZeroPageX   },
        { OP_ROR_ABS, Absolute    },
        { OP_ROR_ABX, AbsoluteX   },
      }
    },
    { "and", &CPU::and_instr, {
        { OP_AND_IMM, Immediate },
        { OP_AND_ZPG, ZeroPage  },
        { OP_AND_ZPX, ZeroPageX },
        { OP_AND_ABS, Absolute  },
        { OP_AND_ABX, AbsoluteX },
        { OP_AND_ABY, AbsoluteY },
        { OP_AND_INX, IndirectX },
        { OP_AND_INY, IndirectY },
      }
    },
    { "eor", &CPU::eor, {
        { OP_EOR_IMM, Immediate },
        { OP_EOR_ZPG, ZeroPage  },
        { OP_EOR_ZPX, ZeroPageX },
        { OP_EOR_ABS, Absolute  },
        { OP_EOR_ABX, AbsoluteX },
        { OP_EOR_ABY, AbsoluteY },
        { OP_EOR_INX, IndirectX },
        { OP_EOR_INY, IndirectY },
      }
    },
    { "ora", &CPU::ora, {
        { OP_ORA_IMM, Immediate },
        { OP_ORA_ZPG, ZeroPage  },
        { OP_ORA_ZPX, ZeroPageX },
        { OP_ORA_ABS, Absolute  },
        { OP_ORA_ABX, AbsoluteX },
        { OP_ORA_ABY, AbsoluteY },
        { OP_ORA_INX, IndirectX },
        { OP_ORA_INY, IndirectY },
      }
    },
    { "bit", &CPU::bit, {
        { OP_BIT_ZPG, ZeroPage },
        { OP_BIT_ABS, Absolute },
      }
    },
    { "cmp", &CPU::cmp, {
        { OP_CMP_IMM, Immediate },
        { OP_CMP_ZPG, ZeroPage  },
        { OP_CMP_ZPX, ZeroPageX },
        { OP_CMP_ABS, Absolute  },
        { OP_CMP_ABX, AbsoluteX },
        { OP_CMP_ABY, AbsoluteY },
        { OP_CMP_INX, IndirectX },
        { OP_CMP_INY, IndirectY },
      }
    },
    { "cpx", &CPU::cpx, {
        { OP_CPX_IMM, Immediate },
        { OP_CPX_ZPG, ZeroPage  },
        { OP_CPX_ABS, Absolute  },
      }
    },
    { "cpy", &CPU::cpy, {
        { OP_CPY_IMM, Immediate },
        { OP_CPY_ZPG, ZeroPage  },
        { OP_CPY_ABS, Absolute  },
      }
    },
    { "inc", &CPU::inc, {
        { OP_INC_ZPG, ZeroPage  },
        { OP_INC_ZPX, ZeroPageX },
        { OP_INC_ABS, Absolute  },
        { OP_INC_ABX, AbsoluteX },
      }
    },
    { "dec", &CPU::dec, {
        { OP_DEC_ZPG, ZeroPage  },
        { OP_DEC_ZPX, ZeroPageX },
        { OP_DEC_ABS, Absolute  },
        { OP_DEC_ABX, AbsoluteX },
      }
    },
    { "inx", &CPU::inx, { { OP_INX_IMP, Implied   }, } },
    { "iny", &CPU::iny, { { OP_INY_IMP, Implied   }, } },
    { "dex", &CPU::dex, { { OP_DEX_IMP, Implied   }, } },
    { "dey", &CPU::dey, { { OP_DEY_IMP, Implied   }, } },
    { "tax", &CPU::tax, { { OP_TAX_IMP, Implied   }, } },
    { "tay", &CPU::tay, { { OP_TAY_IMP, Implied   }, } },
    { "txa", &CPU::txa, { { OP_TXA_IMP, Implied   }, } },
    { "tya", &CPU::tya, { { OP_TYA_IMP, Implied   }, } },
    { "tsx", &CPU::tsx, { { OP_TSX_IMP, Implied   }, } },
    { "txs", &CPU::txs, { { OP_TXS_IMP, Implied   }, } },
    { "clc", &CPU::clc, { { OP_CLC_IMP, Implied   }, } },
    { "cld", &CPU::cld, { { OP_CLD_IMP, Implied   }, } },
    { "cli", &CPU::cli, { { OP_CLI_IMP, Implied   }, } },
    { "sec", &CPU::sec, { { OP_SEC_IMP, Implied   }, } },
    { "sed", &CPU::sed, { { OP_SED_IMP, Implied   }, } },
    { "sei", &CPU::sei, { { OP_SEI_IMP, Implied   }, } },
    { "clv", &CPU::clv, { { OP_CLV_IMP, Implied   }, } },
    { "brk", &CPU::brk, { { OP_BRK_IMP, Implied   }, } },
    { "pha", &CPU::pha, { { OP_PHA_IMP, Implied   }, } },
    { "pla", &CPU::pla, { { OP_PLA_IMP, Implied   }, } },
    { "php", &CPU::php, { { OP_PHP_IMP, Implied   }, } },
    { "plp", &CPU::plp, { { OP_PLP_IMP, Implied   }, } },
    { "rts", &CPU::rts, { { OP_RTS_IMP, Implied   }, } },
    { "nop", &CPU::nop, { { OP_NOP_IMP, Implied   }, } },
    { "rti", &CPU::rti, { { OP_RTI_IMP, Implied   }, } },
    { "bcc", &CPU::bcc, { { OP_BCC_REL, Relative  }, } },
    { "bcs", &CPU::bcs, { { OP_BCS_REL, Relative  }, } },
    { "beq", &CPU::beq, { { OP_BEQ_REL, Relative  }, } },
    { "bne", &CPU::bne, { { OP_BNE_REL, Relative  }, } },
    { "bmi", &CPU::bmi, { { OP_BMI_REL, Relative  }, } },
    { "bpl", &CPU::bpl, { { OP_BPL_REL, Relative  }, } },
    { "bvc", &CPU::bvc, { { OP_BVC_REL, Relative  }, } },
    { "bvs", &CPU::bvs, { { OP_BVS_REL, Relative  }, } },
    { "jsr", &CPU::jsr, { { OP_JSR_ABS, Absolute  }, } },
  };

  public:
  CPU(sc_module_name name) : CPUCore(name) {
    for (const auto& instruction_group : instruction_set) {
      for (const auto& [opcode, _] : instruction_group.instructions) {
        opcode_map[opcode] = &instruction_group;
      }
    }
  }
};
