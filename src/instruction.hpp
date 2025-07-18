#include "types.hpp"

enum OPCodes : opcode_t {
  OP_BRK     = 0x00, // brk
  OP_JMP_ABS = 0x4C, // jmp <addr>
  OP_JMP_IND = 0x6C, // jmp <addr>

  OP_ADC_INX = 0x61, // adc ($addr,X)
  OP_ADC_ZPG = 0x65, // adc $addr
  OP_ADC_IMM = 0x69, // adc #imm
  OP_ADC_ABS = 0x6D, // adc <addr>
  OP_ADC_INY = 0x71, // adc ($addr),Y
  OP_ADC_ZPX = 0x75, // adc $addr,X
  OP_ADC_ABY = 0x79, // adc <addr>,Y
  OP_ADC_ABX = 0x7D, // adc <addr>,X

  OP_STA_INX = 0x81, // sta ($addr,X)
  OP_STA_ZPG = 0x85, // sta $addr
  OP_STA_ABS = 0x8D, // sta <addr>
  OP_STA_INY = 0x91, // sta ($addr),Y
  OP_STA_ZPX = 0x95, // sta $addr,X
  OP_STA_ABY = 0x99, // sta <addr>,Y
  OP_STA_ABX = 0x9D, // sta <addr>,X

  OP_LDA_INX = 0xA1, // lda ($addr,X)
  OP_LDA_ZPG = 0xA5, // lda $addr
  OP_LDA_IMM = 0xA9, // lda #imm
  OP_LDA_ABS = 0xAD, // lda <addr>
  OP_LDA_INY = 0xB1, // lda ($addr),Y
  OP_LDA_ZPX = 0xB5, // lda $addr,X
  OP_LDA_ABY = 0xB9, // lda <addr>,Y
  OP_LDA_ABX = 0xBD, // lda <addr>,X

  OP_NOP     = 0xEA, // nop
};

enum AddressingMode : mem_data_t {
  Implied,
  Immediate,
  ZeroPage,
  ZeroPageX,
  Absolute,
  AbsoluteX,
  AbsoluteY,
  Indirect,
  IndirectX,
  IndirectY
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
