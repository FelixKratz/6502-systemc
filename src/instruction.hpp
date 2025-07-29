#include "types.hpp"

enum OPCodes : opcode_t {
  OP_BRK_IMP = 0x00, // brk

  OP_JMP_ABS = 0x4C, // jmp <addr>
  OP_JMP_IND = 0x6C, // jmp (<addr>)

  OP_ADC_INX = 0x61, // adc ($addr,X)
  OP_ADC_ZPG = 0x65, // adc $addr
  OP_ADC_IMM = 0x69, // adc #imm
  OP_ADC_ABS = 0x6D, // adc <addr>
  OP_ADC_INY = 0x71, // adc ($addr),Y
  OP_ADC_ZPX = 0x75, // adc $addr,X
  OP_ADC_ABY = 0x79, // adc <addr>,Y
  OP_ADC_ABX = 0x7D, // adc <addr>,X

  OP_SBC_INX = 0xE1, // sbc ($addr,X)
  OP_SBC_ZPG = 0xE5, // sbc $addr
  OP_SBC_IMM = 0xE9, // sbc #imm
  OP_SBC_ABS = 0xED, // sbc <addr>
  OP_SBC_INY = 0xF1, // sbc ($addr),Y
  OP_SBC_ZPX = 0xF5, // sbc $addr,X
  OP_SBC_ABY = 0xF9, // sbc <addr>,Y
  OP_SBC_ABX = 0xFD, // sbc <addr>,X

  OP_AND_INX = 0x21, // and ($addr,X)
  OP_AND_ZPG = 0x25, // and $addr
  OP_AND_IMM = 0x29, // and #imm
  OP_AND_ABS = 0x2D, // and <addr>
  OP_AND_INY = 0x31, // and ($addr),Y
  OP_AND_ZPX = 0x35, // and $addr,X
  OP_AND_ABY = 0x39, // and <addr>,Y
  OP_AND_ABX = 0x3D, // and <addr>,X

  OP_EOR_IMM = 0x49, // eor #imm
  OP_EOR_ZPG = 0x45, // eor $addr
  OP_EOR_ZPX = 0x55, // eor $addr,X
  OP_EOR_ABS = 0x4D, // eor <addr>
  OP_EOR_ABX = 0x5D, // eor <addr>,X
  OP_EOR_ABY = 0x59, // eor <addr>,Y
  OP_EOR_INX = 0x41, // eor ($addr,X)
  OP_EOR_INY = 0x51, // eor ($addr),Y

  OP_ORA_IMM = 0x09, // ora #imm
  OP_ORA_ZPG = 0x05, // ora $addr
  OP_ORA_ZPX = 0x15, // ora $addr,X
  OP_ORA_ABS = 0x0D, // ora <addr>
  OP_ORA_ABX = 0x1D, // ora <addr>,X
  OP_ORA_ABY = 0x19, // ora <addr>,Y
  OP_ORA_INX = 0x01, // ora ($addr,X)
  OP_ORA_INY = 0x11, // ora ($addr),Y

  OP_BIT_ZPG = 0x24, // bit $addr
  OP_BIT_ABS = 0x2C, // bit <addr>

  OP_ASL_ACC = 0x0A, // asl A
  OP_ASL_ZPG = 0x06, // asl $addr
  OP_ASL_ZPX = 0x16, // asl $addr,X
  OP_ASL_ABS = 0x0E, // asl <addr>
  OP_ASL_ABX = 0x1E, // asl <addr>,X

  OP_LSR_ACC = 0x4A, // lsr A
  OP_LSR_ZPG = 0x46, // lsr $addr
  OP_LSR_ZPX = 0x56, // lsr $addr,X
  OP_LSR_ABS = 0x4E, // lsr <addr>
  OP_LSR_ABX = 0x5E, // lsr <addr>,X

  OP_ROL_ACC = 0x2A, // rol A
  OP_ROL_ZPG = 0x26, // rol $addr
  OP_ROL_ZPX = 0x36, // rol $addr,X
  OP_ROL_ABS = 0x2E, // rol <addr>
  OP_ROL_ABX = 0x3E, // rol <addr>,X

  OP_ROR_ACC = 0x6A, // ror A
  OP_ROR_ZPG = 0x66, // ror $addr
  OP_ROR_ZPX = 0x76, // ror $addr,X
  OP_ROR_ABS = 0x6E, // ror <addr>
  OP_ROR_ABX = 0x7E, // ror <addr>,X

  OP_CMP_IMM = 0xC9, // cmp #imm
  OP_CMP_ZPG = 0xC5, // cmp $addr
  OP_CMP_ZPX = 0xD5, // cmp $addr,X
  OP_CMP_ABS = 0xCD, // cmp <addr>
  OP_CMP_ABX = 0xDD, // cmp <addr>,X
  OP_CMP_ABY = 0xD9, // cmp <addr>,Y
  OP_CMP_INX = 0xC1, // cmp ($addr,X)
  OP_CMP_INY = 0xD1, // cmp ($addr),Y

  OP_CPX_IMM = 0xE0, // cpx #imm
  OP_CPX_ZPG = 0xE4, // cpx $addr
  OP_CPX_ABS = 0xEC, // cpx <addr>

  OP_CPY_IMM = 0xC0, // cpy #imm
  OP_CPY_ZPG = 0xC4, // cpy $addr
  OP_CPY_ABS = 0xCC, // cpy <addr>

  OP_STA_INX = 0x81, // sta ($addr,X)
  OP_STA_ZPG = 0x85, // sta $addr
  OP_STA_ABS = 0x8D, // sta <addr>
  OP_STA_INY = 0x91, // sta ($addr),Y
  OP_STA_ZPX = 0x95, // sta $addr,X
  OP_STA_ABY = 0x99, // sta <addr>,Y
  OP_STA_ABX = 0x9D, // sta <addr>,X

  OP_STX_ZPG = 0x86, // stx $addr
  OP_STX_ABS = 0x8E, // stx <addr>
  OP_STX_ZPY = 0x96, // stx $addr,Y

  OP_STY_ZPG = 0x84, // sty $addr
  OP_STY_ABS = 0x8C, // sty <addr>
  OP_STY_ZPX = 0x94, // sty $addr,X

  OP_LDA_INX = 0xA1, // lda ($addr,X)
  OP_LDA_ZPG = 0xA5, // lda $addr
  OP_LDA_IMM = 0xA9, // lda #imm
  OP_LDA_ABS = 0xAD, // lda <addr>
  OP_LDA_INY = 0xB1, // lda ($addr),Y
  OP_LDA_ZPX = 0xB5, // lda $addr,X
  OP_LDA_ABY = 0xB9, // lda <addr>,Y
  OP_LDA_ABX = 0xBD, // lda <addr>,X

  OP_LDX_IMM = 0xA2, // ldx #imm
  OP_LDX_ZPG = 0xA6, // ldx $addr
  OP_LDX_ZPY = 0xB6, // ldx $addr,Y
  OP_LDX_ABS = 0xAE, // ldx <addr>
  OP_LDX_ABY = 0xBE, // ldx <addr>,Y

  OP_LDY_IMM = 0xA0, // ldy #imm
  OP_LDY_ZPG = 0xA4, // ldy $addr
  OP_LDY_ZPX = 0xB4, // ldy $addr,X
  OP_LDY_ABS = 0xAC, // ldy <addr>
  OP_LDY_ABX = 0xBC, // ldy <addr>,X

  OP_BCC_REL = 0x90, // bcc #offset
  OP_BCS_REL = 0xB0, // bcs #offset
  OP_BEQ_REL = 0xF0, // beq #offset
  OP_BNE_REL = 0xD0, // bne #offset
  OP_BMI_REL = 0x30, // bmi #offset
  OP_BPL_REL = 0x10, // bpl #offset
  OP_BVC_REL = 0x50, // bvc #offset
  OP_BVS_REL = 0x70, // bvs #offset

  OP_TAX_IMP = 0xAA, // tax
  OP_TAY_IMP = 0xA8, // tay
  OP_TXA_IMP = 0x8A, // txa
  OP_TYA_IMP = 0x98, // tya
  OP_TSX_IMP = 0xBA, // tsx
  OP_TXS_IMP = 0x9A, // txs

  OP_CLC_IMP = 0x18, // clc
  OP_CLD_IMP = 0xD8, // cld
  OP_CLI_IMP = 0x58, // cli
  OP_CLV_IMP = 0xB8, // clv

  OP_SEC_IMP = 0x38, // sec
  OP_SED_IMP = 0xF8, // sed
  OP_SEI_IMP = 0x78, // sei

  OP_INC_ZPG = 0xE6, // inc $addr
  OP_INC_ZPX = 0xF6, // inc $addr,X
  OP_INC_ABS = 0xEE, // inc <addr>
  OP_INC_ABX = 0xFE, // inc <addr>,X

  OP_INX_IMP = 0xE8, // inx

  OP_INY_IMP = 0xC8, // iny

  OP_DEC_ZPG = 0xC6, // dec $addr
  OP_DEC_ZPX = 0xD6, // dec $addr,X
  OP_DEC_ABS = 0xCE, // dec <addr>
  OP_DEC_ABX = 0xDE, // dec <addr>,X

  OP_DEX_IMP = 0xCA, // dex

  OP_DEY_IMP = 0x88, // dey

  OP_NOP_IMP = 0xEA, // nop

  OP_PHA_IMP = 0x48, // pha
  OP_PLA_IMP = 0x68, // pla
  OP_PHP_IMP = 0x08, // php
  OP_PLP_IMP = 0x28, // plp

  OP_JSR_ABS = 0x20, // jsr <addr>
  OP_RTS_IMP = 0x60, // rts
  OP_RTI_IMP = 0x40, // rti
};

enum AddressingMode : mem_data_t {
  Accumulator,
  Implied,
  Immediate,
  ZeroPage,
  ZeroPageX,
  ZeroPageY,
  Absolute,
  AbsoluteX,
  AbsoluteY,
  Indirect,
  IndirectX,
  IndirectY,
  Relative
};

enum MemoryAccessType {
  Read,
  Write,
  ReadModifyWrite
};

struct Operand {
  mem_data_t data;
  AddressingMode mode;
  std::function<void(mem_data_t)> write_back;
};

class CPU;
using InstructionHandler = void(CPU::*)(const AddressingMode);

struct InstructionGroup {
  const std::string name;
  const InstructionHandler handler;
  const std::unordered_map<opcode_t, const AddressingMode> instructions;

  AddressingMode get_mode(opcode_t opcode) const {
    return instructions.at(opcode);
  }
};
