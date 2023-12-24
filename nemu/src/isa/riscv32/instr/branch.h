def_EHelper(beq){
    rtl_jrelop(s, RELOP_EQ, id_src1->preg, id_src2->preg, id_dest->imm);
}

def_EHelper(bne){
    rtl_jrelop(s, RELOP_NE, id_src1->preg, id_src2->preg, id_dest->imm);
}

def_EHelper(blt){
    rtl_jrelop(s, RELOP_LT, id_src1->preg, id_src2->preg, id_dest->imm);
}

def_EHelper(bge){
    rtl_jrelop(s, RELOP_GE, id_src1->preg, id_src2->preg, id_dest->imm);
}

def_EHelper(bltu){
    rtl_jrelop(s, RELOP_LTU, id_src1->preg, id_src2->preg, id_dest->imm);
}

def_EHelper(bgeu){
    rtl_jrelop(s, RELOP_GEU, id_src1->preg, id_src2->preg, id_dest->imm);
}
