def_EHelper(lb) {
  rtl_lms(s, ddest, dsrc1, id_src2->simm, 1);
}

def_EHelper(lh) {
  rtl_lms(s, ddest, dsrc1, id_src2->simm, 2);
}

def_EHelper(lw) {
  rtl_lms(s, ddest, dsrc1, id_src2->simm, 4);
}

def_EHelper(lbu) {
  rtl_lm(s, ddest, dsrc1, id_src2->simm, 1);
}

def_EHelper(lhu) {
  rtl_lm(s, ddest, dsrc1, id_src2->simm, 2);
}

def_EHelper(sw) {
  rtl_sm(s, ddest, dsrc1, id_src2->simm, 4);
}
