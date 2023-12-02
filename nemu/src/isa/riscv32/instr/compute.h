def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(addi){
  rtl_addi(s, ddest, id_src1->preg, id_src2->simm );
}

def_EHelper(auipc){
  rtl_addi(s, ddest, &s->pc, id_src1->imm );
}

def_EHelper(jal){
  rtl_addi(s, ddest, &s->snpc, 0);
  rtl_sext(s, s0, &id_src1->imm, 21);
  s->dnpc = s->pc + *s0;
}