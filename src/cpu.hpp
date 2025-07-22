#include "cpu_core.hpp"

class CPU : public CPUCore<CPU> {
  private:
  /* Implementation of the instruction set */
  void adc(const AddressingMode mode) {
    mem_data_t M = resolve_operand_data(mode);
    uint16_t sum = static_cast<uint16_t>(registers.A) + M + registers.P.C;
    mem_data_t result = static_cast<mem_data_t>(sum);

    registers.P.C = sum > 0xff;
    registers.P.set_zero(result);
    registers.P.set_negative(result);
    registers.P.set_overflow(M, registers.A, result);

    registers.A = result;
  }

  void and_instr(const AddressingMode mode) {
    mem_data_t M = resolve_operand_data(mode);
    registers.A &= M;

    registers.P.set_zero(registers.A);
    registers.P.set_negative(registers.A);
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

  void bcc(const AddressingMode mode) { branch(mode, !registers.P.C); }
  void bcs(const AddressingMode mode) { branch(mode, registers.P.C); }
  void bne(const AddressingMode mode) { branch(mode, !registers.P.Z); }
  void beq(const AddressingMode mode) { branch(mode, registers.P.Z); }
  void bpl(const AddressingMode mode) { branch(mode, !registers.P.N); }
  void bmi(const AddressingMode mode) { branch(mode, registers.P.N); }
  void bvc(const AddressingMode mode) { branch(mode, !registers.P.V); }
  void bvs(const AddressingMode mode) { branch(mode, registers.P.V); }

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
