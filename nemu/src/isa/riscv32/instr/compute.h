def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(addi){
  rtl_addi(s, ddest, id_src1->preg, id_src2->simm );
}

def_EHelper(auipc){
  rtl_addi(s, ddest, &s->pc, id_src1->imm );
}

/* I-Type Compute */

/*Comparison Operation*/
def_EHelper(slti){
  rtl_setrelopi(s, RELOP_LT, ddest, id_src1->preg, id_src2->simm);
}

def_EHelper(sltiu){
  rtl_setrelopi(s, RELOP_LTU, ddest, id_src1->preg, id_src2->simm);
}

/*Bit Operation*/
def_EHelper(xori){
  rtl_xori(s, ddest, id_src1->preg, id_src2->simm);
}

def_EHelper(ori){
  rtl_ori(s, ddest, id_src1->preg, id_src2->simm);
}

def_EHelper(andi){
  rtl_andi(s, ddest, id_src1->preg, id_src2->simm);
}

/*Shift Operation*/
def_EHelper(slli){
  rtl_slli(s, ddest, id_src1->preg, id_src2->simm);
}

def_EHelper(srli){
  rtl_srli(s, ddest, id_src1->preg, id_src2->simm);
}

def_EHelper(srai){
  rtl_srai(s, ddest, id_src1->preg, id_src2->simm);
}
/*R-type Compute*/
/*Arithmetic Operation*/
def_EHelper(add){
  rtl_add(s, ddest, id_src1->preg, id_src2->preg);
}

def_EHelper(sub){
  rtl_sub(s, ddest, id_src1->preg, id_src2->preg);
}

/*Shift Operation*/
def_EHelper(sll){
  rtl_sll(s, ddest, id_src1->preg, id_src2->preg);
}

def_EHelper(srl){
  rtl_srl(s, ddest, id_src1->preg, id_src2->preg);
}

def_EHelper(sra){
  rtl_sra(s, ddest, id_src1->preg, id_src2->preg);
}

/*Comparison Operation*/
def_EHelper(slt){
  rtl_setrelop(s, RELOP_LT, ddest, id_src1->preg, id_src2->preg);
}

def_EHelper(sltu){
  rtl_setrelop(s, RELOP_LTU, ddest, id_src1->preg, id_src2->preg);
}

/*Bit Operation*/
def_EHelper(xor){
  rtl_xor(s, ddest, id_src1->preg, id_src2->preg);
}

def_EHelper(or){
  rtl_or(s, ddest, id_src1->preg, id_src2->preg);
}

def_EHelper(and){
  rtl_and(s, ddest, id_src1->preg, id_src2->preg);
}

/*Jump Control*/

def_EHelper(jal){
  rtl_addi(s, ddest, &s->snpc, 0);
  rtl_sext(s, s0, &id_src1->imm, 21);
  s->dnpc = s->pc + *s0;
}

def_EHelper(jalr){
  rtl_addi(s, ddest, &s->snpc, 0);
  rtl_addi(s, &s->dnpc, id_src1->preg, id_src2->simm);
  s->dnpc &= 0xfffffffe;
}