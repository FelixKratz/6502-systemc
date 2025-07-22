#include <systemc>
#include "adc.hpp"
#include "sta.hpp"
#include "stx.hpp"
#include "sty.hpp"
#include "lda.hpp"
#include "ldx.hpp"
#include "ldy.hpp"
#include "jmp.hpp"
#include "tax.hpp"
#include "tay.hpp"
#include "txa.hpp"
#include "tya.hpp"
#include "nop.hpp"
#include "cl_.hpp"
#include "se_.hpp"
#include "bcc.hpp"
#include "bcs.hpp"
#include "beq.hpp"
#include "bne.hpp"
#include "bmi.hpp"
#include "bpl.hpp"
#include "bvc.hpp"
#include "bvs.hpp"
#include "and.hpp"

std::vector<TestCase> test_cases = {
  // jmp
  { "jmp_abs", test_jmp_abs },
  { "jmp_ind", test_jmp_ind },

  { "jmp_ind_no_bug", test_jmp_ind_no_bug },
  { "jmp_abs_low", test_jmp_abs_low_address },

  // sta
  { "sta_zpg", test_sta_zpg },
  { "sta_zpx", test_sta_zpx },
  { "sta_abs", test_sta_abs },
  { "sta_abx", test_sta_abx },
  { "sta_aby", test_sta_aby },
  { "sta_inx", test_sta_inx },
  { "sta_iny", test_sta_iny },

  { "sta_zpx_wraparound", test_sta_zpx_wraparound },
  { "sta_inx_wraparound", test_sta_inx_wraparound },
  { "sta_iny_page_cross", test_sta_iny_page_cross },

  // stx
  { "stx_zpg", test_stx_zpg },
  { "stx_abs", test_stx_abs },
  { "stx_zpy", test_stx_zpy },

  // sty
  { "sty_zpg", test_sty_zpg },
  { "sty_abs", test_sty_abs },
  { "sty_zpx", test_sty_zpx },

  // lda
  { "lda_abs", test_lda_abs },
  { "lda_aby", test_lda_aby },
  { "lda_abx", test_lda_abx },
  { "lda_inx", test_lda_inx },
  { "lda_iny", test_lda_iny },
  { "lda_zpg", test_lda_zpg },
  { "lda_zpx", test_lda_zpx },
  { "lda_imm", test_lda_imm },

  { "lda_iny_page_cross", test_lda_iny_page_cross },
  { "lda_abx_page_cross", test_lda_abx_page_cross },
  { "lda_imm_zero_flag",  test_lda_imm_zero_flag },

  // ldx
  { "ldx_imm", test_ldx_imm },
  { "ldx_zpg", test_ldx_zpg },
  { "ldx_zpy", test_ldx_zpy },
  { "ldx_abs", test_ldx_abs },
  { "ldx_aby", test_ldx_aby },

  { "ldx_aby_page_cross",    test_ldx_aby_page_cross },
  { "ldx_imm_zero_flag",     test_ldx_imm_zero_flag },
  { "ldx_imm_negative_flag", test_ldx_imm_negative_flag },

  // ldy
  { "ldy_imm",              test_ldy_imm },
  { "ldy_zpg",              test_ldy_zpg },
  { "ldy_zpx",              test_ldy_zpx },
  { "ldy_abs",              test_ldy_abs },
  { "ldy_abx",              test_ldy_abx },

  { "ldy_imm_zero_flag",    test_ldy_imm_zero_flag },
  { "ldy_abx_page_cross",   test_ldy_abx_page_cross },

  // adc
  { "adc_imm", test_adc_imm },
  { "adc_zpg", test_adc_zpg },
  { "adc_abs", test_adc_abs },
  { "adc_abx", test_adc_abx },
  { "adc_zpx", test_adc_zpx },
  { "adc_inx", test_adc_inx },
  { "adc_iny", test_adc_iny },

  { "adc_imm_overflow",   test_adc_imm_overflow },
  { "adc_imm_zero_carry", test_adc_imm_zero_carry },
  { "adc_imm_page_cross", test_adc_abs_page_cross },
  { "adc_iny_page_cross", test_adc_iny_page_cross },
  { "adc_zpg_carry_out",  test_adc_zpg_carry_out },

  // and
  { "and_imm", test_and_imm },
  { "and_imm_zero", test_and_imm_zero },
  { "and_imm_negative", test_and_imm_negative },
  { "and_zpg", test_and_zpg },
  { "and_zpx", test_and_zpx },
  { "and_abs", test_and_abs },
  { "and_abx", test_and_abx },
  { "and_aby", test_and_aby },
  { "and_inx", test_and_inx },
  { "and_iny", test_and_iny },

  // tax
  { "tax_imp",      test_tax_imp },
  { "tax_zero",     test_tax_zero },
  { "tax_negative", test_tax_negative },

  // tay
  { "tay_imp",    test_tay_imp },
  { "tay_zero",   test_tay_zero },
  { "tay_negative", test_tay_negative },

  // txa
  { "txa_imp",       test_txa_imp },
  { "txa_zero",      test_txa_zero },
  { "txa_negative",  test_txa_negative },

  // tya
  { "tya_imp",       test_tya_imp },
  { "tya_zero",      test_tya_zero },
  { "tya_negative",  test_tya_negative },

  // bcc
  { "bcc_taken",      test_bcc_taken },
  { "bcc_not_taken",  test_bcc_not_taken },
  { "bcc_page_cross", test_bcc_page_cross },

  // bcs
  { "bcs_taken",      test_bcs_taken },
  { "bcs_not_taken",  test_bcs_not_taken },

  // beq
  { "beq_taken",      test_beq_taken },
  { "beq_not_taken",  test_beq_not_taken },

  // bne
  { "bne_taken",      test_bne_taken },
  { "bne_not_taken",  test_bne_not_taken },

  // bmi
  { "bmi_taken",      test_bmi_taken },
  { "bmi_not_taken",  test_bmi_not_taken },

  // bpl
  { "bpl_taken",      test_bpl_taken },
  { "bpl_not_taken",  test_bpl_not_taken },

  // bvc
  { "bvc_taken",      test_bvc_taken },
  { "bvc_not_taken",  test_bvc_not_taken },

  // bvs
  { "bvs_taken",      test_bvs_taken },
  { "bvs_not_taken",  test_bvs_not_taken },

  // cl_
  { "clc_imp", test_clc_imp },
  { "cld_imp", test_cld_imp },
  { "cli_imp", test_cli_imp },
  { "clv_imp", test_clv_imp },

  // se_
  { "sec_imp", test_sec_imp },
  { "sed_imp", test_sed_imp },
  { "sei_imp", test_sei_imp },

  // nop
  { "nop", test_nop },
};

int sc_main(int argc, char* argv[]) {
  if (argc == 2) {
    int test_case = 0;
    char name[8] = {};

    if (sscanf(argv[1], "test_case=%d", &test_case) == 1) {
      if (test_case >= 0
          && test_case < test_cases.size()) {
        return !test_cases[test_case].function(test_cases[test_case].name);
      } 
    } else if (sscanf(argv[1], "test_case=%7s", name) == 1) {
      for (size_t i = 0; i < test_cases.size(); i++) {
        if (test_cases[i].name == name) {
          return !test_cases[i].function(test_cases[i].name);
        }
      } 
      std::cout << "Test case: " << name << " not found." << std::endl;
      return 1;
    }  else if (strncmp(argv[1], "--count", 7) == 0) {
      std::cout << test_cases.size() << std::endl;
      return 0;
    }
  }

  std::cout << "Usage: <bin> --count | test_case=<integer> " << std::endl;
  std::cout << "Available Test Cases:" << std::endl;
  for (size_t i = 0; i < test_cases.size(); ++i) {
    std::cout << "  [" << i << "] " << test_cases[i].name << "\n";
  }
  return 0;
}
