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

  void jmp(const AddressingMode mode) {
    registers.pc = fetch_address(mode, Read);
  }

  void brk(const AddressingMode mode) {
    // brk actually takes 7 instructions, which we model later
    for (int i = 0; i < 6; i++) wait();
    registers.P.B = true;
    halted = true;
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

  void clc(const AddressingMode _) { clear_flag(registers.P.C); }
  void cld(const AddressingMode _) { clear_flag(registers.P.D); }
  void cli(const AddressingMode _) { clear_flag(registers.P.I); }
  void clv(const AddressingMode _) { clear_flag(registers.P.V); }

  void sec(const AddressingMode _) { set_flag(registers.P.C); }
  void sed(const AddressingMode _) { set_flag(registers.P.D); }
  void sei(const AddressingMode _) { set_flag(registers.P.I); }

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
    { "tax", &CPU::tax, { { OP_TAX_IMP, Implied   }, } },
    { "tay", &CPU::tay, { { OP_TAY_IMP, Implied   }, } },
    { "txa", &CPU::txa, { { OP_TXA_IMP, Implied   }, } },
    { "tya", &CPU::tya, { { OP_TYA_IMP, Implied   }, } },
    { "clc", &CPU::clc, { { OP_CLC_IMP, Implied   }, } },
    { "cld", &CPU::cld, { { OP_CLD_IMP, Implied   }, } },
    { "cli", &CPU::cli, { { OP_CLI_IMP, Implied   }, } },
    { "sec", &CPU::sec, { { OP_SEC_IMP, Implied   }, } },
    { "sed", &CPU::sed, { { OP_SED_IMP, Implied   }, } },
    { "sei", &CPU::sei, { { OP_SEI_IMP, Implied   }, } },
    { "clv", &CPU::clv, { { OP_CLV_IMP, Implied   }, } },
    { "bcc", &CPU::bcc, { { OP_BCC_REL, Relative  }, } },
    { "bcs", &CPU::bcs, { { OP_BCS_REL, Relative  }, } },
    { "beq", &CPU::beq, { { OP_BEQ_REL, Relative  }, } },
    { "bne", &CPU::bne, { { OP_BNE_REL, Relative  }, } },
    { "bmi", &CPU::bmi, { { OP_BMI_REL, Relative  }, } },
    { "bpl", &CPU::bpl, { { OP_BPL_REL, Relative  }, } },
    { "bvc", &CPU::bvc, { { OP_BVC_REL, Relative  }, } },
    { "bvs", &CPU::bvs, { { OP_BVS_REL, Relative  }, } },
    { "nop", &CPU::nop, { { OP_NOP_IMM, Immediate }, } },
    { "brk", &CPU::brk, { { OP_BRK_IMP, Implied   }, } },
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
