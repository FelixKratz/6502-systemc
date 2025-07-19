#include <systemc>
#include "adc.hpp"
#include "sta.hpp"
#include "lda.hpp"
#include "jmp.hpp"
#include "nop.hpp"

std::vector<TestCase> test_cases = {
  { "jmp_abs", test_jmp_abs },
  { "sta_zpg", test_sta_zpg },
  { "sta_zpx", test_sta_zpx },
  { "lda_abs", test_lda_abs },
  { "lda_aby", test_lda_aby },
  { "lda_abx", test_lda_abx },
  { "lda_inx", test_lda_inx },
  { "lda_iny", test_lda_iny },
  { "lda_zpg", test_lda_zpg },
  { "lda_zpx", test_lda_zpx },
  { "lda_imm", test_lda_imm },
  { "adc_imm", test_adc_imm },
  { "adc_zpg", test_adc_zpg },
  { "adc_abs", test_adc_abs },
  { "adc_abx", test_adc_abx },
  { "adc_zpx", test_adc_zpx },
  { "adc_inx", test_adc_inx },
  { "adc_iny", test_adc_iny },
  { "nop",     test_nop },
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
