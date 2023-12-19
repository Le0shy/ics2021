#ifndef __RTL_PSEUDO_H__
#define __RTL_PSEUDO_H__

#ifndef __RTL_RTL_H__
#error "Should be only included by <rtl/rtl.h>"
#endif

/* RTL pseudo instructions */

static inline def_rtl(li, rtlreg_t *dest, const rtlreg_t imm)
{
  rtl_addi(s, dest, rz, imm);
}

static inline def_rtl(mv, rtlreg_t *dest, const rtlreg_t *src1)
{
  rtl_addi(s, dest, src1, 0);
}

static inline def_rtl(not, rtlreg_t *dest, const rtlreg_t *src1)
{
  // dest <- ~src1
  TODO();
}

static inline def_rtl(neg, rtlreg_t *dest, const rtlreg_t *src1)
{
  // dest <- -src1
  TODO();
}

static inline def_rtl(sext, rtlreg_t *dest, const rtlreg_t *src1, int width)
{
  // dest <- signext(src1[(width * 8 - 1) .. 0])
  //TODO();
  Assert(width > sizeof(word_t) * 4, "Immediate number is invalid");
  /* Obtain the sign bit */
  word_t fetch_bit = 1;
  for (int i = 1; i < width; i =i + 1)
  {
    fetch_bit <<= 1;
  }
  word_t sign_bit = fetch_bit & *src1;

  /* Extend the sign bit */
  *dest = *src1;
  if (sign_bit != 0)
  {
    int signext_count = sizeof(word_t) * 8 - width;
    Log(FMT_WORD, signext_count);
    for (int i = 0; i < signext_count; i = i+ 1)
    {
      sign_bit <<= 1;
      *dest |= sign_bit;
    }
  }
}

static inline def_rtl(zext, rtlreg_t *dest, const rtlreg_t *src1, int width)
{
  // dest <- zeroext(src1[(width * 8 - 1) .. 0])
  TODO();
}

static inline def_rtl(msb, rtlreg_t *dest, const rtlreg_t *src1, int width)
{
  // dest <- src1[width * 8 - 1]
  TODO();
}
#endif
