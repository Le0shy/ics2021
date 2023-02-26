#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  int regs_num = ARRLEN(regs);
  for(int i=0; i<regs_num; i++){
    //use a macro?
    printf(" %-3s  "FMT_WORD " \n",regs[i], cpu.gpr[i]._32);
  }
  //printf("pc  %FMT_PADDR", cpu.pc);
 }

word_t isa_reg_str2val(const char *s, bool *success) {
  int regs_num = ARRLEN(regs);
  for (int i = 0; i < regs_num; i++){
    if (strcmp(regs[i], s) == 0){
      printf(" %-3s  "FMT_WORD " \n",regs[i], cpu.gpr[i]._32);
      *success = 0;
      return cpu.gpr[i]._32 ;
    }
  }
  *success = 1;
  return 0;
}
