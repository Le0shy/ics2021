#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lb) f(lh) f(lw) f(lbu) f(lhu) \
f(slti) f(sltiu) f(xori) f(ori) f(andi) f(slli) f(srli) \
f(srai) f(sw) f(sb) f(sh) f(addi) f(add) f(sub) f(sll) f(srl) f(sra) f(slt) \
f(sltu) f(xor) f(or) f(and) f(auipc) f(jal) f(jalr) f(inv) f(nemu_trap)

def_all_EXEC_ID();
