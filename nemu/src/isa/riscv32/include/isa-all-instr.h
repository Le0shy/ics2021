#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(addi) f(auipc) f(jal) f(inv) f(nemu_trap)

def_all_EXEC_ID();
