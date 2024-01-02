#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  //panic("Not implemented");
  size_t len = 0;
  for(; *s != 0; s += 1, len += 1);
  return len;  
}

char *strcpy(char *dst, const char *src) {
  //panic("Not implemented");
  size_t len = strlen(src);
  memcpy(dst, src, len+1);
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  //panic("Not implemented");
  strcpy(dst + strlen(dst), src);
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  //panic("Not implemented");
  size_t len_1 = strlen(s1);
  size_t len_2 = strlen(s2);
  int res;

  if (len_1 < len_2) {
    res = memcmp(s1, s2, len_1);
    if (res == 0) {
      return -1;
    }
    return res;
  }

  else if (len_1 > len_2) {
    res = memcmp(s1, s2, len_2);
    if(res == 0) {
      return 1;
    }
    return res;
  }

  return memcmp(s1, s2, len_1);
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  //panic("Not implemented");
  uint8_t set = c;
  uint8_t *src = s;
  for(; n !=0; n -= 1){
    src[n-1] = set;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  //panic("Not implemented");
  uint8_t* _dst = dst;
  const uint8_t* _src = src;

  if(dst < src){
    memcpy(dst, src, n);
  }
  else if (src < dst){
    for(; n != 0; n -= 1){
      _dst[n-1] = _src[n-1];
    }
  }

  return dst;
}

void *memcpy(void *to, const void *from, size_t n) {
  //panic("Not implemented");
  uint8_t* dst = to;
  const uint8_t* src = from;
  for(; n != 0; n -= 1) {
    *dst = *src;
    dst += 1;
    src += 1;
  }
  return to;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  //panic("Not implemented");
  const uint8_t* src1 = s1;
  const uint8_t* src2 = s2;
  size_t size = n;
  for(; n != 0; n -= 1){
    if (src1[size - n] == src2[size - n]) continue;
    else {
      return src1[size - n] - src2[size - n];
    }
  }
  return 0;
}

#endif
